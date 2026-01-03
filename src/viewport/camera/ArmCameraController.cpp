#include "ArmCameraController.hpp"
#include <DirectXMath.h>
#include "Camera.hpp"
#include "CameraController.hpp"
#include "common/Math.hpp"
#include "common/Mode.hpp"

#include "common/DebugLog.hpp"
using namespace std;
using namespace Math;
using namespace DirectX;


void ArmCameraController::setTarget(const Vec3& target) 
{
	target_ = target;
	updateView();
}

void ArmCameraController::setDistance(float distance) 
{
	distance_ = distance;
	updateView();
}

void ArmCameraController::setPitch(float pitchDeg) 
{
	pitchDeg_ = pitchDeg;
	updateView();
}

void ArmCameraController::setYaw(float yawDeg) 
{
	yawDeg_ = yawDeg;
	updateView();
}

void ArmCameraController::update(float deltaTime) 
{
	updateView();
}

void ArmCameraController::reset()
{
	pitchDeg_ = initialPitch_;
	yawDeg_ = initialYaw_;
	distance_ = initialDistance_;
	updateView();
}

CameraController::CameraCoreState ArmCameraController::getCoreState() const
{
	CameraController::CameraCoreState result = {};

	float pitchRad = XMConvertToRadians(pitchDeg_);
	float yawRad = XMConvertToRadians(yawDeg_);
	XMVECTOR dir = XMVector3Normalize(XMVectorSet(cosf(pitchRad) * sinf(yawRad), sinf(pitchRad), cosf(pitchRad) * cosf(yawRad), 0.0f));

	XMVECTOR focus = XMLoadFloat3(&target_);
	XMVECTOR cameraPos = focus + dir * distance_;
	
	XMStoreFloat3(&result.position, cameraPos);
	XMStoreFloat3(&result.target, focus);
	result.up = camera_->getUpDirection(); //XMFloat3
	result.distance = distance_;

	return result;
}

void ArmCameraController::onActivate(const CameraController* prevController, CameraMode prevMode) 
{
	if (!camera_ || !prevController || !prevController->getCamera()) 
	{
		reset();
		return;
	}

	//이전 카메라 상태 가져오기
	CameraController::CameraCoreState prevState = prevController->getCoreState();
	XMVECTOR prevPos = XMLoadFloat3(&prevState.position);
	XMVECTOR prevTarget = XMLoadFloat3(&prevState.target);

	XMVECTOR newTarget;
	XMVECTOR dirFromTargetToCam;

	if (prevMode == CameraMode::Target)  //타겟 중심
	{
		newTarget = prevTarget;

		XMVECTOR diff = XMVectorSubtract(prevPos, newTarget);
		dirFromTargetToCam = XMVector3Normalize(diff);
	}
	else  //위치 중심
	{
		XMVECTOR viewDir = XMVectorSubtract(prevTarget, prevPos);
		viewDir = XMVector3Normalize(viewDir);

		newTarget = XMVectorAdd(prevPos, XMVectorScale(viewDir, distance_));
		dirFromTargetToCam = XMVectorScale(viewDir, -1.0f);
	}

	//타겟 적용
	XMStoreFloat3(&target_, newTarget);

	//각도 동기화
	XMFLOAT3 dir;
	XMStoreFloat3(&dir, dirFromTargetToCam);

	pitchDeg_ = XMConvertToDegrees(asinf(dir.y));
	yawDeg_ = XMConvertToDegrees(atan2f(dir.x, dir.z));

	//짐벌락 방지
	if (pitchDeg_ > 89.0f) { pitchDeg_ = 89.0f; }
	if (pitchDeg_ < -89.0f) { pitchDeg_ = -89.0f; }

	camera_->setUpDirectionXM(UP);
	updateView();
}

void ArmCameraController::handleInput(const InputEvent& event) 
{
	visit([&](auto&& ev) {

		using T = decay_t<decltype(ev)>;

		if constexpr (is_same_v<T, MouseDownEvent>)
		{
			if (ev.button_ == 1)
			{
				dragging_ = true;
				lastMousePos_ = ev.pos_;
			}
			else if (ev.button_ == 2) 
			{
				panning_ = true;
				lastMousePos_ = ev.pos_;
			}
		}
		else if constexpr (is_same_v<T, MouseUpEvent>)
		{
			if (ev.button_ == 1) { dragging_ = false; }
			else if (ev.button_ == 2) { panning_ = false; }
		}
		else if constexpr (is_same_v<T, MouseMoveEvent>)
		{
			if (dragging_)
			{
				int dx = ev.pos_.x - lastMousePos_.x;
				int dy = ev.pos_.y - lastMousePos_.y;
				lastMousePos_ = ev.pos_;

				yawDeg_ += dx * rotationSpeed_;
				pitchDeg_ += dy * rotationSpeed_;

				//pitch제한 (짐벌락 방지)
				if (pitchDeg_ < -89.0f) pitchDeg_ = -89.0f;
				if (pitchDeg_ > 89.0f) pitchDeg_ = 89.0f;
				updateView();
			}
			else if (panning_) 
			{
				int dx = ev.pos_.x - lastMousePos_.x;
				int dy = ev.pos_.y - lastMousePos_.y;
				lastMousePos_ = ev.pos_;

				float pitchRad = XMConvertToRadians(pitchDeg_);
				float yawRad = XMConvertToRadians(yawDeg_);

				XMVECTOR forward = XMVectorSet(cosf(pitchRad) * sinf(yawRad), sinf(pitchRad), cosf(pitchRad) * cosf(yawRad), 0.0f);
				forward = XMVector3Normalize(forward);

				XMVECTOR right = XMVector3Cross(UP, forward);
				right = XMVector3Normalize(right);

				XMVECTOR camUp = XMVector3Cross(forward, right);
				camUp = XMVector3Normalize(camUp);

				float moveSpeed = panSpeed_ * distance_;
				XMVECTOR translation = (right * (float)dx * moveSpeed) + (camUp * (float)dy * moveSpeed);

				//XMFLOAT3 tPos;
				//XMStoreFloat3(&tPos, translation);
				//tPos.y *= 0.5f; //패닝의 y축 이동 보정
				//translation = XMLoadFloat3(&tPos);

				XMVECTOR currentTarget = XMLoadFloat3(&target_);
				currentTarget = XMVectorAdd(currentTarget, translation);
				XMStoreFloat3(&target_, currentTarget);
				updateView();
			}
		}
		else if constexpr (is_same_v<T, MouseWheelEvent>) 
		{
			float zoomAmount = ev.delta_ * zoomSpeed_ * distance_ * 0.1f; //거리 비례 보정
			distance_ -= zoomAmount;

			if (distance_ < 0.1f) distance_ = 0.1f;
			updateView();
		}
	}, event);
}


void ArmCameraController::updateView() 
{
	if (distance_ < 0.1f) distance_ = 0.1f;

	float pitchRad = XMConvertToRadians(pitchDeg_);
	float yawRad = XMConvertToRadians(yawDeg_);
	XMVECTOR dir = XMVector3Normalize(XMVectorSet(cosf(pitchRad) * sinf(yawRad), sinf(pitchRad), cosf(pitchRad) * cosf(yawRad), 0.0f));

	//3. 카메라 위치 = 타겟 위치 + 방향 * 거리
	XMVECTOR focus = XMLoadFloat3(&target_);
	XMVECTOR cameraPos = focus + dir * distance_;
	XMVECTOR up = camera_->getUpDirectionXM();

	camera_->setPositionXM(cameraPos);
	camera_->setTargetXM(focus);
	camera_->setUpDirectionXM(up);
}
