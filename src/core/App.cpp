#pragma once
#include "App.hpp"
#include <chrono>
#include <cassert>
#include <vector>
#include "common/ElementID.hpp"
#include "common/Fonts.hpp"
#include "AppConfig.hpp"
#include "command/CommandStack.hpp"
#include "command/passiveObject/CmdCreatePrimitive.hpp"

#include "object/action/SceneObjectActionHandler.hpp"
#include "viewport/ui/GizmoController.hpp"
#include "manager/ManagerInclude.hpp"
#include "coordinator/CoordinatorInclude.hpp"
#include "ui/UIInclude.hpp"
#include "event/EventInclude.hpp"
#include "event/appEvent/flow/SceneDataChangedEvent.hpp"

#include "ui/window/environmentConfig/EnvironmentConfigController.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/dx11/DX11Renderer.hpp"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "common/DebugLog.hpp"
using namespace std;


App::App() : 
	inputHandler_(make_unique<InputHandler>()),
	inputEditorHandler_(make_unique<InputEventEditorHandler>()),
	sceneObjActionHandler_(make_unique<SceneObjectActionHandler>()),
	envConfigController_(make_unique<EnvConfig::EnvironmentConfigController>()),

	appUIManager_(make_unique<AppUIManager>()),
	shortcutManager_(make_unique<ShortcutManager>()),
	floatingWindowManager_(make_unique<FloatingWindowManager>()),
	sceneObjManager_(make_unique<SceneObjectManager>()),

	cameraCoordinator_(make_unique<CameraCoordinator>()),
	renderCoordinator_(make_unique<RenderCoordinator>()),
	resourceCoordinator_(make_unique<ResourceCoordinator>()),
	selectionCoordinator_(make_unique<SelectionCoordinator>()),
	viewStateCoordinator_(make_unique<ViewStateCoordinator>()),

	renderer_(make_unique<DX11Renderer>()),
	mainMenuBarUI_(make_unique<MainMenuBarUI>()) {}

App::~App() = default;


bool App::initialize(void* hwnd) 
{
	AppConfigData appData = AppConfig::load();

	hwnd_ = static_cast<HWND>(hwnd);
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
	assert(shortcutManager_ && "없음, 초기화 실패");
	if (shortcutManager_)
	{
		shortcutManager_->initialize();
	}
	assert(appUIManager_ && "없음, 초기화 실패");
	if (appUIManager_)
	{
		appUIManager_->initialize();
	}
	assert(cameraCoordinator_ && "없음, 초기화 실패");
	if (cameraCoordinator_)
	{
		cameraCoordinator_->initialize();
	}
	assert(resourceCoordinator_ && "없음, 초기화 실패");
	if (resourceCoordinator_)
	{
		resourceCoordinator_->initialize(renderer_->getDevice(), renderer_->getDeviceContext());
	}
	assert(sceneObjManager_ && "없음, 초기화 실패");
	if (sceneObjManager_)
	{
		SceneObjectContext context;
		context.renderer = renderer_.get();
		context.matManager = resourceCoordinator_->getMaterialManager();
		sceneObjManager_->initialize(context);
	}
	assert(renderCoordinator_ && "없음, 초기화 실패");
	if (renderCoordinator_)
	{
		RenderContext context;
		context.renderer = renderer_.get();
		context.camCoordinator = cameraCoordinator_.get();
		context.passiveObjCoordinator = sceneObjManager_->getPassiveCoordinator();
		context.resourceCoordinator = resourceCoordinator_.get();

		renderCoordinator_->initialize(context);
	}
	assert(envConfigController_ && "envConfigController 없음, 초기화 실패");
	if (envConfigController_)
	{
		EnvConfig::EnvironmentContext context;
		context.lightManager = renderCoordinator_->getLightManager();
		
		envConfigController_->initialize(context);
	}
	assert(selectionCoordinator_ && "비었음, 초기화 실패");
	if (selectionCoordinator_)
	{
		SelectionContext context;
		context.camCoord = cameraCoordinator_.get();
		context.sceneObjManager = sceneObjManager_.get();

		selectionCoordinator_->initialize(context);
	}
	assert(floatingWindowManager_ && "floatingWindowManager 없음, 초기화 실패");
	if (floatingWindowManager_)
	{
		FloatingWindowContext context;
		context.lightManager = renderCoordinator_->getLightManager();
		context.cameraManager = cameraCoordinator_->getViewportCameraManager();
		context.passiveObjCoordinator = sceneObjManager_->getPassiveCoordinator();

		FloatingConfigData floatingData;
		floatingData = appData.floatingConfig;

		floatingWindowManager_->initialize(context, floatingData);
	}
	assert(viewStateCoordinator_ && "viewStateCoordinator 없음, 초기화 실패");
	if (viewStateCoordinator_)
	{
		viewStateCoordinator_->initialize();
	}
	assert(sceneObjActionHandler_ && "sceneObjectActionHandler 없음, 초기화 실패");
	if (sceneObjActionHandler_) 
	{
		SceneObjectActionContext context;
		context.sceneObjManager = sceneObjManager_.get();
		context.selectionCoord = selectionCoordinator_.get();

		sceneObjActionHandler_->initialize(context);
	}
	assert(mainMenuBarUI_ && "mainMenuBarUI 없음, 초기화 실패");
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

		this->onScreenResize(width, height);
	}

	auto sceneChangeID = AppEventSubscriber::get().subscribe<SceneDataChangedEvent>([this](const SceneDataChangedEvent& event)
		{
			onSceneDataChanged(event);
		});
	appEventSubID_.push_back(sceneChangeID);

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
		draw();
		ImGui::Render();


		// 3. RENDER (실제 화면 출력)
		render();
	}
}

void App::update(float deltaTime) 
{
	if (inputHandler_)  //KeyPressed를 KeyDown/Up/Hold로 번역
	{
		inputHandler_->update();
	}

	cameraCoordinator_->update(deltaTime);
	sceneObjManager_->update(deltaTime);

	Math::Ray mouseRay = {};
	ImVec2 mousePos = ImGui::GetMousePos();
	ImVec2 vpSize = ImGui::GetMainViewport()->Size;
	if (cameraCoordinator_)
	{
		mouseRay = cameraCoordinator_->convertScreenPointToRay(mousePos.x, mousePos.y, vpSize.x, vpSize.y);
	}

	while (!InputEventQueue::get().isEmpty())
	{
		InputEvent event = InputEventQueue::get().pop();
		if (inputEditorHandler_) //InputEvent를 InputEditorEvent로 번역
		{
			inputEditorHandler_->onInputEvent(event, mouseRay);
		}
	}

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
	while (!EditorEventQueue::get().isEmpty())
	{
		EditorEventVariant event = EditorEventQueue::get().pop();
		EditorEventSubscriber::get().dispatch(event);
	}

	//이벤트 처리 후 적용
	if (renderCoordinator_)
	{
		renderCoordinator_->updateTransforms();
		renderCoordinator_->update(deltaTime, mouseRay);
	}
}

void App::draw() 
{
	appUIManager_->draw(); //공통UI 구성
	cameraCoordinator_->draw();
	viewStateCoordinator_->draw();
	mainMenuBarUI_->draw();
	floatingWindowManager_->draw();
	renderCoordinator_->drawImGui();
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

void App::onScreenResize(int width, int height) 
{
	if (width == 0 || height == 0) { return; }

	width_ = width;
	height_ = height;

	AppEventPublisher::get().publish(WindowSizeChangedEvent{ width, height });
}

void App::onSceneDataChanged(const SceneDataChangedEvent& event)
{
	switch (event.type)
	{
	case SceneDataType::DataChanged:
	{
		if (currentSceneTitle_.empty() || currentSceneTitle_.back() != '*') { currentSceneTitle_ += "*"; }
		break;
	}
	case SceneDataType::FileSaved:
	{
		currentSceneTitle_ = filesystem::path(event.path).filename().string();
		break;
	}
	case SceneDataType::FileLoaded:
	{
		currentSceneTitle_ = event.path.empty() ? "" : filesystem::path(event.path).filename().string();
		break;
	}
	default:
		break;
	}

	std::string title = "JIINY's 3DTools";
	if (!currentSceneTitle_.empty()) { title += " - " + currentSceneTitle_; }
	SetWindowTextA(hwnd_, title.c_str());
}
