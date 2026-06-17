#include "FloatingWindowManager.hpp"
#include <vector>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include "command/CommandStack.hpp"
#include "core/manager/editor/ViewportCameraManager.hpp"
#include "core/coordinator/scene/PassiveObjectCoordinator.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/ui/CreatePopupChangedEvent.hpp"
#include "event/appEvent/ui/CreatePopupRequestedEvent.hpp"
#include "event/appEvent/ui/EnvironmentConfigPopupRequestedEvent.hpp"
#include "event/appEvent/ui/EnvironmentConfigPopupChangedEvent.hpp"
#include "event/appEvent/ui/MaterialPopupRequestedEvent.hpp"
#include "event/appEvent/ui/MaterialPopupChangedEvent.hpp"
#include "event/appEvent/state/WindowSizeChangedEvent.hpp"
#include "ui/window/create/CreatePanel.hpp"
#include "ui/window/environmentConfig/EnvironmentConfig.hpp"
#include "ui/window/material/MaterialPanel.hpp"

#include "event/uiEvent/UIEventPublisher.hpp"
#include "event/uiEvent/UIEventSubscriber.hpp"
#include "event/uiEvent/viewport/CameraInfoRequestedEvent.hpp"
#include "event/uiEvent/viewport/CameraInfoChangedEvent.hpp"
#include "ui/preference/CameraInfoUI.hpp"
using namespace std;


FloatingWindowManager::FloatingWindowManager() :
	create_(make_unique<Create::CreatePanel>()),
	envConfig_(make_unique<EnvConfig::EnvironmentConfig>()),
	material_(make_unique<MaterialEditor::MaterialPanel>()),
	cameraInfoUI_(make_unique<CameraInfoUI>()) {}

FloatingWindowManager::~FloatingWindowManager() = default;

bool FloatingWindowManager::initialize(const FloatingWindowContext& context, const FloatingConfigData& data)
{
	assert(context.lightManager && "비었습니다. 초기화 실패");
	assert(context.cameraManager && "비었습니다. 초기화 실패");
	assert(context.passiveObjCoordinator && "비었습니다. 초기화 실패");
	assert(context.materialManager && "비었습니다. 초기화 실패");
	if (!context.lightManager || !context.cameraManager || !context.passiveObjCoordinator || !context.materialManager)
	{ 
		return false; 
	}

	camManager_ = context.cameraManager;

	floatingInfo_ = data.floatingInfo;
	drawMap_["Create"] = [this](bool& isVisible) { create_->draw(isVisible); };
	drawMap_["Environment Config"] = [this](bool& isVisible) { envConfig_->draw(isVisible); };
	drawMap_["Material"] = [this](bool& isVisible) { material_->draw(isVisible); };
	drawMap_["Camera Info"] = [this](bool& isVisible) { material_->draw(isVisible); };

	if (!create_->initialize(context.passiveObjCoordinator)) { return false; }
	if (!envConfig_->initialize(context.lightManager)) { return false; };
	if (!material_->initialize(context.materialManager)) { return false; }
	cameraInfoUI_->initialize(camManager_);


	//콜백 구독 예약
	auto createID = AppEventSubscriber::get().subscribe<CreatePopupRequestedEvent>([this](const CreatePopupRequestedEvent& event)
		{
			this->setCreateVisibility(event.isVisible);
		});
	appEventSubID_.push_back(createID);

	auto envConfigID = AppEventSubscriber::get().subscribe<EnvironmentConfigPopupRequestedEvent>([this](const EnvironmentConfigPopupRequestedEvent& event)
		{
			this->setEnvironmentConfigVisibility(event.isVisible);
		});
	appEventSubID_.push_back(envConfigID);

	auto matEditorID = AppEventSubscriber::get().subscribe<MaterialPopupRequestedEvent>([this](const MaterialPopupRequestedEvent& event)
		{
			this->setMaterialVisibility(event.isVisible);
		});
	appEventSubID_.push_back(matEditorID);


	ImVec2 wSize = ImGui::GetMainViewport()->Size;
	prevWidth_ = wSize.x;
	prevHeight_ = wSize.y;

	auto winResizeID = AppEventSubscriber::get().subscribe<WindowSizeChangedEvent>([this](const WindowSizeChangedEvent& event)
		{
			this->onWindowSizeChanged(event.width, event.height);
		});
	appEventSubID_.push_back(winResizeID);

	auto camInfoSubID = UIEventSubscriber::get().subscribe<CameraInfoRequestedEvent>([this](const CameraInfoRequestedEvent& event)
	{
		this->setCameraInfoVisibility(event.isVisible);
	});
	uiEventSubID_.push_back(camInfoSubID);

	return true;
}

void FloatingWindowManager::draw()
{
	for (auto& w : floatingInfo_)
	{
		if (!w.isVisible) { continue; }

		auto it = drawMap_.find(w.windowName);
		if (it != drawMap_.end()) { it->second(w.isVisible); }
	}
}

FloatingWindowInfo* FloatingWindowManager::findWindowInfo(const std::string& configKey)
{
	auto it = find_if(floatingInfo_.begin(), floatingInfo_.end(), [&](const FloatingWindowInfo& w)
		{ 
			return w.configKey == configKey; 
		});
	return it != floatingInfo_.end() ? &(*it) : nullptr;
}

void FloatingWindowManager::setCreateVisibility(bool isVisible) 
{
	auto* w = findWindowInfo("showCreate");
	if (!w || w->isVisible == isVisible) { return; }

	w->isVisible = isVisible;
	AppEventPublisher::get().publish(CreatePopupChangedEvent{ isVisible });
}

void FloatingWindowManager::setEnvironmentConfigVisibility(bool isVisible) 
{
	auto* w = findWindowInfo("showEnvConfig");
	if (!w || w->isVisible == isVisible) { return; }

	w->isVisible = isVisible;
	AppEventPublisher::get().publish(EnvironmentConfigPopupChangedEvent{ isVisible });
}

void FloatingWindowManager::setMaterialVisibility(bool isVisible)
{
	auto* w = findWindowInfo("showMaterialEditor");
	if (!w || w->isVisible == isVisible) { return; }

	w->isVisible = isVisible;
	AppEventPublisher::get().publish(MaterialPopupChangedEvent{ isVisible });
}

void FloatingWindowManager::setCameraInfoVisibility(bool isVisible)
{
	auto* w = findWindowInfo("showCamInfo");
	if (!w || w->isVisible == isVisible) { return; }

	w->isVisible = isVisible;
	UIEventPublisher::get().publish(CameraInfoChangedEvent{ isVisible });
}

void FloatingWindowManager::toggleCameraInfo()
{
	auto* w = findWindowInfo("showCamInfo");
	if (!w) { return; }
	setCameraInfoVisibility(!w->isVisible);
}

bool FloatingWindowManager::isCamInfoVisible()
{
	auto* w = findWindowInfo("showCamInfo");
	if (!w) { return false; }

	return w->isVisible;
}

void FloatingWindowManager::onWindowSizeChanged(int width, int height)
{
	float newWidth = static_cast<float>(width);
	float newHeight = static_cast<float>(height);
	float deltaW = newWidth - prevWidth_;
	float deltaH = newHeight - prevHeight_;
	float midX = newWidth * 0.5f;
	float midY = newHeight * 0.5f;


	for (const auto& w : floatingInfo_)
	{
		if (!w.isVisible) { continue; }
		
		ImGuiWindow* win = ImGui::FindWindowByName(w.windowName.c_str());
		if (!win) { continue; }

		ImVec2 pos = win->Pos;
		ImVec2 size = win->Size;
		float centerX = pos.x + size.x * 0.5f;
		float centerY = pos.y + size.y * 0.5f;

		ImVec2 newPos = pos;
		if (centerX > midX) { newPos.x += deltaW; }
		if (centerY > midY) { newPos.y += deltaH; }

		if (newPos.x != pos.x || newPos.y != pos.y)
		{
			ImGui::SetWindowPos(w.windowName.c_str(), newPos);
		}
	}

	prevWidth_ = newWidth;
	prevHeight_ = newHeight;
}
