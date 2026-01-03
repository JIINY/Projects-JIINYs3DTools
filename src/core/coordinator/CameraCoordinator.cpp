#include "CameraCoordinator.hpp"
#include "core/manager/editor/ViewportCameraManager.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/state/AppModeChangedEvent.hpp"
#include "event/appEvent/state/CameraModeChangedEvent.hpp"
#include "event/InputEventType.hpp"


void CameraCoordinator::initialize() 
{
	//콜백 구독 예약
	auto appModeEventID = AppEventSubscriber::get().subscribe<AppModeChangedEvent>([this](const AppModeChangedEvent& event)
		{
			this->currentMode_ = event.newMode_;
			this->viewportCameraManager_.setCameraModeForAppMode(event.newMode_);
		});
	AppEventSubID_.push_back(appModeEventID);

	auto camModeEventID = AppEventSubscriber::get().subscribe<CameraModeChangedEvent>([this](const CameraModeChangedEvent& event)
		{
			this->viewportCameraManager_.setCameraMode(event.newCam_, event.newView_);
		});
	AppEventSubID_.push_back(camModeEventID);

	auto windowResizeEventID = AppEventSubscriber::get().subscribe<WindowSizeChangedEvent>([this](const WindowSizeChangedEvent& event)
		{
			this->viewportCameraManager_.updateProjection(event.width, event.height);
		});
	AppEventSubID_.push_back(windowResizeEventID);
}

void CameraCoordinator::draw() 
{
	viewportCameraManager_.draw();
}

void CameraCoordinator::update(float deltaTime)
{
	viewportCameraManager_.update(deltaTime);
}

void CameraCoordinator::shutdown() 
{
	for (const auto& id : AppEventSubID_) 
	{
		AppEventSubscriber::get().unsubscribe(id);
	}

	AppEventSubID_.clear();
}

void CameraCoordinator::handleInput(const InputEvent& event) 
{
	viewportCameraManager_.handleInput(event);
}
