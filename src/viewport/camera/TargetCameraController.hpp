#pragma once
#include <Windows.h>
#include "CameraController.hpp"
#include "common/Math.hpp"
#include "common/Mode.hpp"

class TargetCameraController : public CameraController {
public:
	TargetCameraController();

	void update(float deltaTime) override;
	void handleInput(const InputEvent& event) override;
	void reset();

	void onActivate(const CameraController* prevController, CameraMode prevMode) override;
	void setTargetPosition(const Math::Vec3& pos);

	CameraController::CameraCoreState getCoreState() const override;
	Math::Vec3 getTarget() const override { return target_; }
	float getDistance() const override { return distance_; }
	float getPitch() const override { return pitchDeg_; }
	float getYaw() const override { return yawDeg_; }


private:
	static constexpr float initialPitch_ = 20.0f;
	static constexpr float initialYaw_ = 0.0f;
	static constexpr float initialDistance_ = 10.0f;

	float minDistance_ = 2.0f;
	float maxDistance_ = 30.0f;
	float minPitch_ = -20.0f;
	float maxPitch_ = 60.0f;

	Math::Vec3 target_ = { 0.0f, 0.0f, 0.0f };

	float distance_;
	float pitchDeg_;
	float yawDeg_;

	float rotationSpeed_ = 0.3f;
	float zoomSpeed_ = 1.0f;

	bool dragging_ = false;
	POINT lastMousePos_ = { 0, 0 };
	void updateView();
};
