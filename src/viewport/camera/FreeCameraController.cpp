#include "FreeCameraController.hpp"
#include <Windows.h>
#include <cmath>
#include <DirectXMath.h>
#include <algorithm>
#include "Camera.hpp"
#include "CameraController.hpp"
#include "common/Math.hpp"
#include "common/Mode.hpp"

#include "common/DebugLog.hpp"
using namespace std;
using namespace Math;
using namespace DirectX;


FreeCameraController::FreeCameraController() : pitchDeg_(initialPitch_), yawDeg_(initialYaw_) {}

void FreeCameraController::reset() 
{
	pitchDeg_ = initialPitch_;
	yawDeg_ = initialYaw_;
}

void FreeCameraController::setPosition(const Math::Vec3& pos) 
{
	camera_->setPosition(pos);
	updateView();
}

CameraController::CameraCoreState FreeCameraController::getCoreState() const
{
	CameraController::CameraCoreState result = {};

	result.position = camera_->getPosition();
	result.target = camera_->getTarget();
	result.up = camera_->getUpDirection();
	
	XMVECTOR pos = XMLoadFloat3(&result.position);
	XMVECTOR target = XMLoadFloat3(&result.target);
	result.distance = XMVectorGetX(XMVector3Length(XMVectorSubtract(pos, target)));

	return result;
}

void FreeCameraController::onActivate(const CameraController* prevController, CameraMode prevMode)
{
	if (!camera_ || !prevController || !prevController->getCamera()) 
	{
		resetMovementState();
		updateView();
		return;
	}

	CameraController::CameraCoreState prevState = prevController->getCoreState();

	XMVECTOR prevPos = XMLoadFloat3(&prevState.position);
	XMVECTOR prevTarget = XMLoadFloat3(&prevState.target);
	XMVECTOR up = XMLoadFloat3(&prevState.up);

	XMMATRIX viewMat = XMMatrixLookAtLH(prevPos, prevTarget, up);
	XMMATRIX worldMat = XMMatrixInverse(nullptr, viewMat);

	XMFLOAT3 zAxis; //forward벡터 worldMat.r[2]
	XMStoreFloat3(&zAxis, worldMat.r[2]);

	pitchDeg_ = XMConvertToDegrees(asinf(zAxis.y));
	yawDeg_ = XMConvertToDegrees(atan2f(-zAxis.x, -zAxis.z));

	camera_->setPosition(prevState.position);
	camera_->setUpDirection(XMFLOAT3(0.0f, 1.0f, 0.0f));

	resetMovementState();
	updateView();
	return;
}

void FreeCameraController::resetMovementState() 
{
	velocity_ = { 0.0f, 0.0f, 0.0f };
	moveForward_ = moveBackward_ = moveLeft_ = moveRight_ = false;
	yawDragging_ = false;
	pitchDragging_ = false;
}

void FreeCameraController::handleInput(const InputEvent& event) 
{
	constexpr int KEY_W = 568;
	constexpr int KEY_A = 546;
	constexpr int KEY_S = 564;
	constexpr int KEY_D = 549;
	constexpr int KEY_E = 550;
	constexpr int KEY_Q = 562;

	visit([&](auto&& ev) {

		using T = decay_t<decltype(ev)>;

		if constexpr (is_same_v<T, MouseDownEvent>)
		{
			if (ev.button_ == 1)
			{
				yawDragging_ = true;
				lastMousePos_ = ev.pos_;
			}
			else if (ev.button_ == 2) 
			{
				pitchDragging_ = true;
				lastMousePos_ = ev.pos_;
			}

		}
		else if constexpr (is_same_v<T, MouseUpEvent>) 
		{
			if (ev.button_ == 1)
			{
				yawDragging_ = false;
			}
			else if(ev.button_ == 2)
			{
				pitchDragging_ = false;
			}
		}
		else if constexpr (is_same_v<T, MouseMoveEvent>) 
		{
			if (yawDragging_ || pitchDragging_)
			{
				int dx = ev.pos_.x - lastMousePos_.x;
				int dy = ev.pos_.y - lastMousePos_.y;
				lastMousePos_ = ev.pos_;

				if (yawDragging_) { yawDeg_ += dx * rotationSpeed_; }
				if (pitchDragging_) 
				{ 
					pitchDeg_ -= dy * rotationSpeed_; 
					pitchDeg_ = clamp(pitchDeg_, -89.0f, 89.0f); //짐벌락 방지
				}

				updateView();
			}
		}
		else if constexpr (is_same_v<T, KeyDownEvent>) 
		{
			switch (ev.keyCode_) {
			case KEY_W: moveForward_ = true; break;
			case KEY_S: moveBackward_ = true; break;
			case KEY_A: moveLeft_ = true; break;
			case KEY_D: moveRight_ = true; break;
			case KEY_E: moveUp_ = true; break;
			case KEY_Q: moveDown_ = true; break;
			}
		}
		else if constexpr (is_same_v<T, KeyUpEvent>)
		{
			switch (ev.keyCode_) {
			case KEY_W: moveForward_ = false; break;
			case KEY_S: moveBackward_ = false; break;
			case KEY_A: moveLeft_ = false; break;
			case KEY_D: moveRight_ = false; break;
			case KEY_E: moveUp_ = false; break;
			case KEY_Q: moveDown_ = false; break;
			}
		}
	}, event);

}

void FreeCameraController::update(float deltaTime) 
{
	XMMATRIX yawRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(pitchDeg_), XMConvertToRadians(yawDeg_), 0.0f);

	XMVECTOR forward = XMVector3TransformNormal(FORWARD, yawRot);
	XMVECTOR right = XMVector3TransformNormal(RIGHT, yawRot);

	XMVECTOR moveDir = XMVectorZero();
	if (moveForward_) moveDir = XMVectorAdd(moveDir, forward);
	if (moveBackward_) moveDir = XMVectorSubtract(moveDir, forward);
	if (moveRight_) moveDir = XMVectorAdd(moveDir, right);
	if (moveLeft_) moveDir = XMVectorSubtract(moveDir, right);
	if (moveUp_) moveDir = XMVectorAdd(moveDir, UP);
	if (moveDown_) moveDir = XMVectorSubtract(moveDir, UP);

	//속도 적용
	if (XMVector3LengthSq(moveDir).m128_f32[0] > 0.001f) 
	{
		moveDir = XMVector3Normalize(moveDir);
		XMVECTOR vel = XMVectorScale(moveDir, moveSpeed_);
		XMStoreFloat3((XMFLOAT3*)&velocity_, vel);
	}
	else 
	{
		velocity_ = { 0.0f, 0.0f, 0.0f };
	}

	//위치 갱신
	XMVECTOR currentVel = XMLoadFloat3((XMFLOAT3*)&velocity_);
	XMVECTOR pos = XMLoadFloat3((XMFLOAT3*)&camera_->getPosition());

	//위치 = 현재 위치 + (속도 * 시간)
	pos = XMVectorAdd(pos, XMVectorScale(currentVel, deltaTime));

	Vec3 newPos;
	XMStoreFloat3((XMFLOAT3*)&newPos, pos);
	camera_->setPosition(newPos);

	updateView();
}

void FreeCameraController::updateView() 
{
	//1. pitch(X축 회전), yaw(y축 회전) 적용
	XMMATRIX rotX = XMMatrixRotationX(XMConvertToRadians(pitchDeg_));
	XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(yawDeg_));
	XMMATRIX rot = XMMatrixMultiply(rotX, rotY);

	//2. 기준 정면 벡터를 회전시켜 실제 시선 방향 계산
	XMVECTOR dir = XMVector3TransformCoord(FORWARD, rot);

	//3. 카메라 위치 + 시선 방향 -> 타겟 위치 계산
	XMVECTOR cameraPos = camera_->getPositionXM();
	XMVECTOR focusPoint = XMVectorAdd(cameraPos, dir);

	//4.Up벡터 (월드 상단 기준)
	//XMVECTOR up = UP;

	//5. 뷰 행렬 계산
	XMMATRIX view = XMMatrixLookAtLH(cameraPos, focusPoint, UP); //Debug용 차후 삭제 필요

	//6. 카메라 적용
	camera_->setPositionXM(cameraPos);
	camera_->setTargetXM(focusPoint);
	camera_->setUpDirectionXM(UP);
}
