#pragma once
#include <memory>
#include <map>
#include <optional>
#include "common/Mode.hpp"
#include "viewport/camera/CameraInclude.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"


struct WindowSizeChangedEvent;

class App;
class AppEventPublisher;
class ViewportCamUI;


class ViewportCameraManager {
public:
	ViewportCameraManager();
	~ViewportCameraManager();

	void initializer();
	void draw();
	void update(float deltaTime);

	void updateProjection(int width, int height);

	void setCameraModeForAppMode(AppMode mode);
	void setCameraMode(CameraMode newCam, ViewMode newView);
	CameraMode getCameraMode() const { return currentCam_; }
	CameraMode getLastCam() const { return lastCam_; }
	ViewMode getViewMode() const { return currentView_; }
	const Camera& getActiveCam() const { return activeCam_; }
	CameraController* getActiveCamController() const { return activeCamController_; }


private:
	std::vector<AppEventSubscriptionID> appEventSubID_;
	std::unique_ptr<ViewportCamUI> viewportCamUI_;

	Camera activeCam_;
	CameraController* activeCamController_ = nullptr;
	std::map<CameraMode, std::unique_ptr<CameraController>> camController_;
	std::optional<FixedAngleCameraController> fixedCamController_;

	CameraMode currentCam_ = CameraMode::FreeArm;
	ViewMode currentView_ = ViewMode::None;
	CameraMode lastCam_ = currentCam_;

	bool isFixedAngleCam(CameraMode cam);
	void setActiveCamController(CameraMode prevMode);
	void onWindowSizeChanged(const WindowSizeChangedEvent& event);
};
