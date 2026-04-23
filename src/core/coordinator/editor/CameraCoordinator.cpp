#include "CameraCoordinator.hpp"
#include <memory>
#include "core/manager/editor/ViewportCameraManager.hpp"
#include "viewport/camera/Camera.hpp"

#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/state/AppModeChangedEvent.hpp"
#include "event/appEvent/state/CameraModeChangedEvent.hpp"
using namespace std;


CameraCoordinator::CameraCoordinator() : viewportCameraManager_(make_unique<ViewportCameraManager>()) {}
CameraCoordinator::~CameraCoordinator() = default;

void CameraCoordinator::initialize() 
{
	//콜백 구독 예약
	auto appModeEventID = AppEventSubscriber::get().subscribe<AppModeChangedEvent>([this](const AppModeChangedEvent& event)
		{
			this->currentMode_ = event.newMode_;
			this->viewportCameraManager_->setCameraModeForAppMode(event.newMode_);
		});
	appEventSubID_.push_back(appModeEventID);

	auto camModeEventID = AppEventSubscriber::get().subscribe<CameraModeChangedEvent>([this](const CameraModeChangedEvent& event)
		{
			this->viewportCameraManager_->setCameraMode(event.newCam_, event.newView_);
		});
	appEventSubID_.push_back(camModeEventID);

	auto windowResizeEventID = AppEventSubscriber::get().subscribe<WindowSizeChangedEvent>([this](const WindowSizeChangedEvent& event)
		{
			this->viewportCameraManager_->updateProjection(event.width, event.height);
		});
	appEventSubID_.push_back(windowResizeEventID);
}

void CameraCoordinator::draw() 
{
	viewportCameraManager_->draw();
}

void CameraCoordinator::update(float deltaTime)
{
	viewportCameraManager_->update(deltaTime);
}

void CameraCoordinator::shutdown() 
{
	for (const auto& id : appEventSubID_) 
	{
		AppEventSubscriber::get().unsubscribe(id);
	}
	appEventSubID_.clear();
}

Math::Ray CameraCoordinator::convertScreenPointToRay(float screenX, float screenY, float viewportX, float viewportY)
{
	Camera activeCam = viewportCameraManager_->getActiveCam();
	return activeCam.convertScreenPointToRay(screenX, screenY, viewportX, viewportY);
}
