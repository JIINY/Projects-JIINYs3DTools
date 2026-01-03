#include "FixedAngleCameraController.hpp"
#include <Windows.h>
#include <variant>
#include <type_traits>
#include <DirectXMath.h>
#include "Camera.hpp"
#include "CameraController.hpp"
#include "common/Math.hpp"
#include "common/Mode.hpp"
#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/state/CameraModeChangedEvent.hpp"
using namespace std;
using namespace Math;
using namespace DirectX;


FixedAngleCameraController::FixedAngleCameraController(ViewMode view) : FixedAngleCameraController(getPitchFromView(view), getYawFromView(view)) {}
FixedAngleCameraController::FixedAngleCameraController(float pitchDeg, float yawDeg) : pitchDeg_(pitchDeg), yawDeg_(yawDeg)
{
}

void FixedAngleCameraController::setTarget(const Vec3& target) 
{
	target_ = target;
	updateView();
}

void FixedAngleCameraController::setDistance(float distance) 
{
	distance_ = distance;
	updateView();
}

void FixedAngleCameraController::update(float deltaTime) 
{
	//고정 각도라 별도 업데이트 필요 없음
	//panOffset이나 distance가 바뀌면 updateView()로 처리됨
}

void FixedAngleCameraController::handleInput(const InputEvent& event) 
{
	visit([&](auto&& ev) {

		using T = decay_t<decltype(ev)>;

		if constexpr (is_same_v<T, MouseDownEvent>) 
		{
			if (ev.button_ == 1) //right
			{
				dragging_ = true;
				lastMousePos_ = ev.pos_;
			}
			else if (ev.button_ == 2) 
			{
				dragging_ = false; //드래그가 시작되면 바꿈
				lastMousePos_ = ev.pos_;
			}
		}
		else if constexpr (is_same_v<T, MouseUpEvent>) 
		{
			if (ev.button_ == 1)
			{
				dragging_ = false;
			}
		}
		else if constexpr (is_same_v<T, MouseMoveEvent>) 
		{
			if (dragging_)
			{
				int dx = ev.pos_.x - lastMousePos_.x;
				int dy = ev.pos_.y - lastMousePos_.y;
				lastMousePos_ = ev.pos_;

				//카메라의 현재 벡터 계산
				float pitchRad = XMConvertToRadians(pitchDeg_);
				float yawRad = XMConvertToRadians(yawDeg_);

				XMVECTOR forward = XMVectorSet(cosf(pitchRad) * sinf(yawRad), sinf(pitchRad), cosf(pitchRad) * cosf(yawRad), 0.0f);
				forward = XMVector3Normalize(forward);

				XMVECTOR right = XMVector3Cross(UP, forward);
				right = XMVector3Normalize(right);

				XMVECTOR camUp = XMVector3Cross(forward, right);
				camUp = XMVector3Normalize(camUp);

				//이동량 계산
				float speed = panSpeed_ * distance_;
				XMVECTOR translation = (right * (float)dx * speed) + (camUp * (float)dy * speed);

				//타겟 이동
				XMVECTOR currentTarget = XMLoadFloat3(&target_);
				currentTarget = XMVectorAdd(currentTarget, translation);
				XMStoreFloat3(&target_, currentTarget);

				updateView();
			}
			else if (GetKeyState(VK_MBUTTON) < 0) //휠버튼 체크
			{
				int dx = abs(ev.pos_.x - lastMousePos_.x);
				int dy = abs(ev.pos_.y - lastMousePos_.y);

				if (dx > 2 || dy > 2) 
				{
					AppEventPublisher::get().publish(CameraModeChangedEvent(CameraMode::FreeArm, ViewMode::None));
				}
			}
		}
		else if constexpr (is_same_v<T, MouseWheelEvent>) 
		{
			distance_ -= ev.delta_ * zoomSpeed_ * distance_ * 0.1f;
			if (distance_ < 0.1f) distance_ = 0.1f;
			updateView();
		}
	}, event);
}

void FixedAngleCameraController::updateView() 
{
	if (distance_ < 0.1f) distance_ = 0.1f;

	//1. 고정된 pitch/yaw로 방향벡터 계산 (라디안 변환)
	float pitchRad = XMConvertToRadians(pitchDeg_);
	float yawRad = XMConvertToRadians(yawDeg_);

	//2. 방향 벡터 계산 (구면 좌표계 → 직교 좌표계)
	XMVECTOR dir = XMVector3Normalize(XMVectorSet(cosf(pitchRad) * sinf(yawRad), sinf(pitchRad), cosf(pitchRad) * cosf(yawRad), 0.0f));

	//3. 카메라 위치 = 타겟 - (방향 * 거리)
	XMVECTOR focusPoint = XMLoadFloat3(&target_);
	XMVECTOR cameraPos = focusPoint - (dir * distance_);

	//4.Up벡터 (월드 상단 기준)
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); //Y-up
	if (fabs(pitchDeg_) > 89.0f) 
	{
		up = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); //Z-up, 짐벌락 방지
	}

	//5. 카메라 적용
	camera_->setPositionXM(cameraPos);
	camera_->setTargetXM(focusPoint);
	camera_->setUpDirectionXM(up);
}

void FixedAngleCameraController::onActivate(const CameraController* prevController, CameraMode prevMode) 
{
	if (!camera_ || !prevController || !prevController->getCamera()) 
	{
		updateView();
		return;
	}

	//이전 카메라의 target가져오기
	CameraController::CameraCoreState prevState = prevController->getCoreState();
	XMVECTOR prevTarget = XMLoadFloat3(&prevState.target);
	XMVECTOR prevPos = XMLoadFloat3(&prevState.position);

	XMStoreFloat3(&target_, prevTarget);

	//거리 계산
	XMVECTOR distVec = XMVectorSubtract(prevPos, prevTarget);
	distance_ = XMVectorGetX(XMVector3Length(distVec));

	if (distance_ < 0.1f) { distance_ = 0.1f; }

	updateView();
}
