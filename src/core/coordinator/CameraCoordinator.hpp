#pragma once
#include <memory>
#include <vector>
#include "../manager/editor/ViewportCameraManager.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/InputEventType.hpp"

class AppEventSubscriber;


class CameraCoordinator 
{
public:
	ViewportCameraManager* getViewportCameraManager() { return &viewportCameraManager_; }

	void initialize();
	void draw();
	void update(float deltaTime);
	void shutdown();
	void handleInput(const InputEvent& event);

private:
	AppMode currentMode_ = AppMode::Edit;
	ViewportCameraManager viewportCameraManager_;

	std::vector<AppEventSubscriptionID> AppEventSubID_;
};
