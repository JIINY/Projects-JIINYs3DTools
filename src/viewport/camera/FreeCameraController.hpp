#pragma once
#include <Windows.h>
#include "CameraController.hpp"
#include "common/Math.hpp"
#include "common/Mode.hpp"


class FreeCameraController : public CameraController {
public:
	FreeCameraController();

	void update(float deltaTime) override;
	void reset();
	void resetMovementState();

	void setPosition(const Math::Vec3& pos);
	void onActivate(const CameraController* prevController, CameraMode prevMode) override;
	void onDeactivate() override;

	CameraController::CameraCoreState getCoreState() const override;
	float getPitch() const override { return pitchDeg_; }
	float getYaw() const override { return yawDeg_; }


private:
	static constexpr float initialPitch_ = 3.28f;
	static constexpr float initialYaw_ = 5.07f;

	float pitchDeg_;
	float yawDeg_;
	Math::Vec3 velocity_ = { 0.0f, 0.0f, 0.0f };

	static constexpr float rotationSpeed_ = 0.15f;
	static constexpr float moveSpeed_ = 10.0f;

	bool yawDragging_ = false;
	bool pitchDragging_ = false;
	POINT lastMousePos_ = { 0, 0 };

	bool moveForward_ = false;
	bool moveBackward_ = false;
	bool moveLeft_ = false;
	bool moveRight_ = false;
	bool moveUp_ = false;
	bool moveDown_ = false;

	void updateView();
	void onKeyDowned(const KeyDownEditorEvent& event);
	void onKeyUpped(const KeyUpEditorEvent& event);
	void onMouseDowned(const MouseDownEditorEvent& event);
	void onMouseUpped(const MouseUpEditorEvent& event);
	void onMouseWheeled(const MouseWheelEditorEvent& event);
	void onMouseMoved(const MouseMoveEditorEvent& event);
};
