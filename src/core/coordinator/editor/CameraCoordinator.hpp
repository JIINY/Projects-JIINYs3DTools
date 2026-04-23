#pragma once
#include <memory>
#include <vector>
#include "common/Math.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"

class AppEventSubscriber;
class ViewportCameraManager;

class CameraCoordinator 
{
public:
	CameraCoordinator();
	~CameraCoordinator();

	void initialize();
	void draw();
	void update(float deltaTime);
	void shutdown();

	ViewportCameraManager* getViewportCameraManager() { return viewportCameraManager_.get(); }
	Math::Ray convertScreenPointToRay(float screenX, float screenY, float viewportX, float viewportY);


private:
	std::vector<AppEventSubscriptionID> appEventSubID_;
	std::unique_ptr<ViewportCameraManager> viewportCameraManager_;
	AppMode currentMode_ = AppMode::Edit;
};
