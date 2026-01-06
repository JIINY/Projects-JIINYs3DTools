#pragma once
#include "App.hpp"
#include <chrono>
#include <cassert>
#include "common/ElementID.hpp"
#include "common/Fonts.hpp"
#include "AppConfig.hpp"
#include "ManagerInclude.hpp"
#include "coordinator/CameraCoordinator.hpp"
#include "coordinator/RenderCoordinator.hpp"
#include "coordinator/ViewStateCoordinator.hpp"
#include "coordinator/ResourceCoordinator.hpp"
#include "ui/UIInclude.hpp"

#include "event/appEvent/AppEventQueue.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/uiEvent/UIEventQueue.hpp"
#include "event/uiEvent/UIEventSubscriber.hpp"
#include "event/InputEventType.hpp"
#include "io/InputEventQueue.hpp"
#include "io/InputHandler.hpp"
#include "scene/SceneSerializer.hpp"
#include "ui/window/environmentConfig/EnvironmentConfigSerializer.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/dx11/DX11Renderer.hpp"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
using namespace std;


App::App() : 
	inputHandler_(std::make_unique<InputHandler>()),
	appUIManager_(std::make_unique<AppUIManager>()),

	shortcutManager_(std::make_unique<ShortcutManager>()),
	floatingWindowManager_(std::make_unique<FloatingWindowManager>()),

	cameraCoordinator_(std::make_unique<CameraCoordinator>()),
	renderCoordinator_(std::make_unique<RenderCoordinator>()),
	resourceCoordinator_(std::make_unique<ResourceCoordinator>()),
	viewStateCoordinator_(std::make_unique<ViewStateCoordinator>()),

	environmentConfigSerializer_(std::make_unique<EnvConfig::EnvironmentConfigSerializer>()),
	sceneSerializer_(std::make_unique<Scene::SceneSerializer>()),
	renderer_(std::make_unique<DX11Renderer>()),
	mainMenuBarUI_(std::make_unique<MainMenuBarUI>()) {}

App::~App() {}


bool App::initialize(void* hwnd) 
{
	AppConfigData appData = AppConfig::load();

	if (!renderer_->initialize(hwnd)) { return false; }

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 bgColor = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = bgColor;
	style.Colors[ImGuiCol_ChildBg] = bgColor;
	style.Colors[ImGuiCol_PopupBg] = bgColor;

	style.FrameRounding = 4.0f;
	style.GrabRounding = 2.0f;

	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

	style.Colors[ImGuiCol_Button] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);

	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);

	style.Colors[ImGuiCol_Header] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);

	ImGui_ImplWin32_Init((HWND)hwnd);
	ImGui_ImplDX11_Init(renderer_->getDevice(), renderer_->getDeviceContext());

	Fonts::Init();
	ImGui_ImplDX11_CreateDeviceObjects();

	//초기화 호출
	if (appUIManager_)
	{
		appUIManager_->initialize();
	}

	if (cameraCoordinator_)
	{
		cameraCoordinator_->initialize();
	}
	if (!resourceCoordinator_->initialize(renderer_->getDevice(), renderer_->getDeviceContext()))
	{
		assert(0 && "[에러] ResourceCoordinator 초기화 실패\n");
		return false;
	}
	if (!renderCoordinator_->initialize(renderer_.get(), cameraCoordinator_.get()))
	{
		assert(0 && "[에러] RenderCoordinator 초기화 실패\n");
		return false;
	}

	if (environmentConfigSerializer_)
	{
		EnvConfig::EnvConfigContext envContext;
		envContext.lightManager = renderCoordinator_->getLightManager();

		environmentConfigSerializer_->initialize(envContext);
		environmentConfigSerializer_->deserialize("src/defaults/DefaultEnvironmentConfig.json");
	}
	if (sceneSerializer_)
	{

	}
	if (floatingWindowManager_)
	{
		LightManager* lightManager = renderCoordinator_->getLightManager();
		ViewportCameraManager* camManager = cameraCoordinator_->getViewportCameraManager();

		FloatingWindowContext context;
		context.lightManager = lightManager;
		context.cameraManager = camManager;
		context.envSerializer = environmentConfigSerializer_.get();

		FloatingConfigData floatingData;
		floatingData = appData.floatingConfig;

		floatingWindowManager_->initialize(context, floatingData);
	}
	if (viewStateCoordinator_) 
	{
		viewStateCoordinator_->initialize();
	}
	if (mainMenuBarUI_)
	{
		mainMenuBarUI_->initialize();
	}


	//초기 상태를 강제 동기화(첫 프레임 버그용)
	cameraCoordinator_->getViewportCameraManager()->setCameraModeForAppMode(AppMode::Edit);

	//시작시 화면크기 카메라에 동기화
	HWND handle = static_cast<HWND>(hwnd);
	RECT rc;
	if (GetClientRect(handle, &rc)) 
	{
		float width = static_cast<float>(rc.right - rc.left);
		float height = static_cast<float>(rc.bottom - rc.top);

		WindowSizeChangedEvent initEvent = { width, height };
		AppEventSubscriber::get().dispatch(initEvent);
	}

	return true;
}

void App::run() 
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	auto prev = chrono::steady_clock::now(); //프레임 타이머 초기화
	bool running = true;

	while (running) 
	{
		//메세지 전부 소진
		while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) 
		{
			if (msg.message == WM_QUIT) { running = false; break; }
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (!running) { break; }

		//ImGui 프레임 시작
		ImGui_ImplWin32_NewFrame();
		ImGui_ImplDX11_NewFrame();
		ImGui::NewFrame();

		// 1. UPDATE (로직만 처리)
		//deltaTime 계산
		auto now = chrono::steady_clock::now();
		float deltaTime = chrono::duration<float>(now - prev).count();
		prev = now;

		//App 로직 밑 이벤트 처리
		update(deltaTime);


		// 2. DRAW (화면에 보일 요소 구성)
		handleInput();
		draw();
		ImGui::Render();


		// 3. RENDER (실제 화면 출력)
		render();
	}
}

void App::handleInput() 
{
	ImGuiIO& io = ImGui::GetIO();
	for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; ++i) 
	{
		ImGuiKey key = static_cast<ImGuiKey>(i);

		//IsKeyPressed는 이번 프레임에 처음 눌렸는가를 확인
		if (ImGui::IsKeyPressed(key, false))
		{
			InputEventQueue::get().push(KeyPressedEvent{ i });
		}
	}

	//마우스 입력 처리
}

void App::update(float deltaTime) 
{
	if (inputHandler_)  //KeyPressed를 KeyDown/Up/Hold로 번역
	{
		inputHandler_->update(InputEventQueue::get());
	}
	while (!InputEventQueue::get().isEmpty()) 
	{
		InputEvent event = InputEventQueue::get().pop();
		if (inputHandler_) 
		{
			inputHandler_->handleEvent(event, InputEventQueue::get());
		}

		if (shortcutManager_) 
		{
			shortcutManager_->processInputEvent(event);
		}

		if (cameraCoordinator_) 
		{
			cameraCoordinator_->handleInput(event);
		}
	}


	cameraCoordinator_->update(deltaTime);

	//이벤트 처리
	while (!AppEventQueue::get().isEmpty())
	{
		AppEventVariant event = AppEventQueue::get().pop();
		AppEventSubscriber::get().dispatch(event);
	}
	while (!UIEventQueue::get().isEmpty()) 
	{
		UIEventVariant event = UIEventQueue::get().pop();
		UIEventSubscriber::get().dispatch(event);
	}
}

void App::draw() 
{
	appUIManager_->draw(); //공통UI 구성
	cameraCoordinator_->draw();
	viewStateCoordinator_->draw();
	mainMenuBarUI_->draw();
	floatingWindowManager_->draw();
}

void App::render()
{
	const float bgColor[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	renderer_->beginFrame(bgColor);

	renderCoordinator_->render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	renderer_->endFrame();
}


void App::shutdown()
{
	AppConfigData data;

	if (floatingWindowManager_) { data.floatingConfig = floatingWindowManager_->getCurrentState(); }

	AppConfig::save(data);

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	renderer_->shutdown();
}
