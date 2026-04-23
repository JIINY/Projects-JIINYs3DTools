#include "ViewportCameraManager.hpp"
#include <map>
#include <optional>
#include "common/Mode.hpp"
#include "core/App.hpp"
#include "viewport/camera/CameraInclude.hpp"
#include "viewport/ui/ViewportCamUI.hpp"
#include "imgui.h"

#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/state/CameraModeChangedEvent.hpp"
#include "event/appEvent/state/WindowSizeChangedEvent.hpp"

#include "common/DebugLog.hpp"
using namespace std;


ViewportCameraManager::ViewportCameraManager() 
	: viewportCamUI_(std::make_unique<ViewportCamUI>())
{
	camController_[CameraMode::FreeArm] = std::make_unique<ArmCameraController>();
	camController_[CameraMode::Target] = std::make_unique<TargetCameraController>();
	camController_[CameraMode::FreeFly] = std::make_unique<FreeCameraController>();
}
ViewportCameraManager::~ViewportCameraManager() = default;


void ViewportCameraManager::initializer()
{
	viewportCamUI_->initialize(currentCam_, currentView_);
	activeCamController_->onActivate(nullptr, CameraMode::Count);

	auto sizeChangeID = AppEventSubscriber::get().subscribe<WindowSizeChangedEvent>([this](const WindowSizeChangedEvent& event)
		{
			this->onWindowSizeChanged(event);
		});
	appEventSubID_.push_back(sizeChangeID);
}

void ViewportCameraManager::setCameraModeForAppMode(AppMode mode)
{
	switch (mode) {
	case AppMode::Edit: 
	{
		setCameraMode(CameraMode::FreeArm, ViewMode::None);
		break;
	}
	case AppMode::Play:
	{
		setCameraMode(CameraMode::Target, ViewMode::None);
		break;
	}
	}
}

void ViewportCameraManager::setCameraMode(CameraMode newCam, ViewMode newView)
{
	CameraMode oldMode = currentCam_;

	if (currentCam_ == newCam && currentView_ == newView) 
	{
		if (!activeCamController_) 
		{
			auto& cam = activeCam_;
			auto pos = cam.getPosition();
			auto target = cam.getTarget();

			setActiveCamController(oldMode);
		}
		return;
	}

	AppEventPublisher::get().publish(CameraModeChangedEvent(newCam, newView));

	//FixedStart
	if (!isFixedAngleCam(currentCam_) && isFixedAngleCam(newCam) && newView != ViewMode::None)
	{
		lastCam_ = currentCam_;
		currentCam_ = newCam;
		currentView_ = newView;
		setActiveCamController(oldMode);
		return;
	}

	//FixedToFixed
	if (isFixedAngleCam(currentCam_) && currentView_ != newView)
	{
		currentView_ = newView;
		setActiveCamController(oldMode);
		return;
	}

	//FixedToLastCam
	if (isFixedAngleCam(currentCam_) && currentView_ == newView)
	{
		currentCam_ = lastCam_;
		currentView_ = ViewMode::None;
		setActiveCamController(oldMode);
		return;
	}

	//FixedToOther
	if (isFixedAngleCam(currentCam_) && !isFixedAngleCam(newCam))
	{
		currentCam_ = newCam;
		currentView_ = ViewMode::None;
		setActiveCamController(oldMode);
		return;
	}

	currentCam_ = newCam;
	currentView_ = newView;

	auto& cam = activeCam_;
	auto pos = cam.getPosition();
	auto target = cam.getTarget();
	setActiveCamController(oldMode);
}

bool ViewportCameraManager::isFixedAngleCam(CameraMode cam)
{
	return cam == CameraMode::FixedAngle;
}

void ViewportCameraManager::setActiveCamController(CameraMode prevMode)
{
	CameraController* prevController = activeCamController_;

	if (currentCam_ != CameraMode::FixedAngle)
	{
		auto it = camController_.find(currentCam_);
		if (it != camController_.end())
		{
			activeCamController_ = it->second.get();
		}
		else 
		{
			activeCamController_ = nullptr;
		}
	}
	else 
	{
		fixedCamController_.emplace(currentView_);
		activeCamController_ = &(*fixedCamController_);
	}

	if (prevController)
	{
		prevController->onDeactivate();
	}
	if (activeCamController_)
	{
		activeCamController_->setCamera(&activeCam_);
		activeCamController_->onActivate(prevController, prevMode);
	}
}

void ViewportCameraManager::update(float deltaTime)
{
	if (auto* activeCam = getActiveCamController())
	{
		activeCam->update(deltaTime);
		viewportCamUI_->setString(currentCam_, currentView_);
	}
}

void ViewportCameraManager::draw()
{
	viewportCamUI_->draw();
}

void ViewportCameraManager::updateProjection(int width, int height) 
{
	if (height <= 0) { return; }

	float aspect = static_cast<float>(width) / static_cast<float>(height);
	activeCam_.setAspect(aspect);
}

void ViewportCameraManager::onWindowSizeChanged(const WindowSizeChangedEvent& event) 
{
	activeCam_.setWidth(event.width);
	activeCam_.setHeight(event.height);

	this->updateProjection(event.width, event.height);
}
