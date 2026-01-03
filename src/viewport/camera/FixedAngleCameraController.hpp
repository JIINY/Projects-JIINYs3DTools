#pragma once
#include <Windows.h>
#include "CameraController.hpp"
#include "common/Math.hpp"
#include "common/Mode.hpp"


class FixedAngleCameraController : public CameraController {
public:
	FixedAngleCameraController(ViewMode view);
	FixedAngleCameraController(float pitchDeg, float yawDeg);

	void update(float deltaTime) override;
	void handleInput(const InputEvent& event) override;
	void onActivate(const CameraController* prevController, CameraMode prevMode) override;

	void setTarget(const Math::Vec3& t);
	void setDistance(float d);


private:
	Math::Vec3 target_ = { 0.0f, 0.0f, 0.0f };

	float distance_ = 10.0f;
	const float pitchDeg_;
	const float yawDeg_;

	float zoomSpeed_ = 1.0f;
	float panSpeed_ = 0.01f;

	bool dragging_ = false;
	POINT lastMousePos_ = { 0, 0 };

	void updateView();
};
