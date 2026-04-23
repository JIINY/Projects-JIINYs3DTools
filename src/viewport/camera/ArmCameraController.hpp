#pragma once
#include<Windows.h>
#include "CameraController.hpp"
#include "common/Math.hpp"
#include "common/Mode.hpp"


class ArmCameraController : public CameraController {
public:
	void update(float deltaTime) override;	
	void reset();

	void onActivate(const CameraController* prevController, CameraMode prevMode) override;
	void onDeactivate() override;
	void setTarget(const Math::Vec3& t);
	void setDistance(float d);
	void setPitch(float pitchDeg);
	void setYaw(float yawDeg);

	CameraController::CameraCoreState getCoreState() const override;
	Math::Vec3 getTarget() const override { return target_; }
	float getDistance() const override { return distance_; }
	float getPitch() const override { return pitchDeg_; }
	float getYaw() const override { return yawDeg_; }


private:
	static constexpr float initialDistance_ = 10.0f;
	static constexpr float initialPitch_ = 30.0f;
	static constexpr float initialYaw_ = -135.0f;

	Math::Vec3 target_ = { 0.0f, 0.0f, 0.0f };
	float distance_ = 10.0f;
	float pitchDeg_ = 30.0f;
	float yawDeg_ = -135.0f;

	float zoomSpeed_ = 1.0f;
	float rotationSpeed_ = 0.1f;
	bool panning_ = false;
	float panSpeed_ = 0.002f;

	bool dragging_ = false;
	POINT lastMousePos_ = { 0, 0 };

	void updateView();
	void onMouseDowned(const MouseDownEditorEvent& event);
	void onMouseUpped(const MouseUpEditorEvent& event);
	void onMouseWheeled(const MouseWheelEditorEvent& event);
	void onMouseMoved(const MouseMoveEditorEvent& event);
};
