#pragma once
#include <memory>
#include <map>
#include <optional>
#include "common/Mode.hpp"
#include "viewport/camera/CameraInclude.hpp"
#include "event/InputEventType.hpp"

class App;
class AppEventPublisher;
class ViewportCamUI;


class ViewportCameraManager {
public:
	ViewportCameraManager();
	~ViewportCameraManager();

	void setCameraModeForAppMode(AppMode mode);
	void setCameraMode(CameraMode newCam, ViewMode newView);
	CameraMode getCameraMode() const { return currentCam_; }
	CameraMode getLastCam() const { return lastCam_; }
	ViewMode getViewMode() const { return currentView_; }
	const Camera& getActiveCam() const { return activeCam_; }
	CameraController* getActiveCamController() const { return activeCamController_; }

	void initializer(AppEventPublisher* appEventPublisher);
	void draw();
	void update(float deltaTime);
	void updateProjection(float width, float height);
	void handleInput(const InputEvent& event);

private:	
	Camera activeCam_;
	CameraController* activeCamController_ = nullptr;
	std::unique_ptr<ViewportCamUI> viewportCamUI_;

	std::map<CameraMode, std::unique_ptr<CameraController>> camController_;
	std::optional<FixedAngleCameraController> fixedCamController_;

	CameraMode currentCam_ = CameraMode::FreeArm;
	ViewMode currentView_ = ViewMode::None;
	CameraMode lastCam_ = currentCam_;

	AppEventPublisher* appEventPublisher_;

	bool isFixedAngleCam(CameraMode cam);
	void setActiveCamController(CameraMode prevMode);
};
