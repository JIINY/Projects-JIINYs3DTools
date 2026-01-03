#include "TargetCameraController.hpp"
#include <Windows.h>
#include <DirectXMath.h>
#include "Camera.hpp"
#include "CameraController.hpp"
#include "common/Math.hpp"
#include "common/Mode.hpp"
using namespace std;
using namespace Math;
using namespace DirectX;

TargetCameraController::TargetCameraController() : pitchDeg_(initialPitch_), yawDeg_(initialYaw_), distance_(initialDistance_) {}

void TargetCameraController::setTargetPosition(const Vec3& pos) 
{
	target_ = pos;
	updateView();
}

void TargetCameraController::update(float deltaTime) 
{
	updateView();
}

void TargetCameraController::reset() 
{
	pitchDeg_ = initialPitch_;
	yawDeg_ = initialYaw_;
	distance_ = initialDistance_;
	updateView();
}

CameraController::CameraCoreState TargetCameraController::getCoreState() const
{
	CameraController::CameraCoreState result = {};

	XMStoreFloat3(&result.position, camera_->getPositionXM());
	XMStoreFloat3(&result.target, camera_->getTargetXM());
	result.up = camera_->getUpDirection();
	result.distance = distance_;

	return result;
}

void TargetCameraController::onActivate(const CameraController* prevController, CameraMode prevMode) 
{
	auto player = nullptr;
	if (!camera_ || !player)
	{
		reset();
		return;
	}

	//XMVECTOR playerPos = player->getPositionXM();
	//XMVECTOR playerForward = player->getForwardVectorXM();

	//뒤꽁무니 뷰로 계산, 퀄업할 필요 있음
	//this->target_ = playerPos;
	//this->distance_ = initialDistance_;
	//this->pitchDeg_ = initialPitch_;

	//yaw는 캐릭터 앞방향의 반대로 돌려서 계산
	///float playerYaw = atan2f(playerForward.x, playerForward.z);
	//this-> yawDeg_ = XMConvertToDegrees(playerYaw) + 180.0f

	updateView();
}

void TargetCameraController::handleInput(const InputEvent& event) 
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

				yawDeg_ += dx * rotationSpeed_;
				pitchDeg_ -= dy * rotationSpeed_;

				if (pitchDeg_ < minPitch_) pitchDeg_ = minPitch_;
				if (pitchDeg_ > maxPitch_) pitchDeg_ = maxPitch_;
				updateView();
			}
		}
		else if constexpr (is_same_v<T, MouseWheelEvent>) 
		{
			distance_ -= ev.delta_ * zoomSpeed_;

			if (distance_ < minDistance_) distance_ = minDistance_;
			if (distance_ > maxDistance_) distance_ = maxDistance_;
			updateView();
		}
	}, event);
}


void TargetCameraController::updateView() 
{
#ifdef _DEBUG
	if (!camera_)
	{
		OutputDebugStringW(L"[Target] camera == nullptr\n");
		return;
	}
#endif

	if (distance_ < 0.1f) distance_ = 0.1f;

	//1. 표준 오비탈 로직
	float pitchRad = XMConvertToRadians(pitchDeg_);
	float yawRad = XMConvertToRadians(yawDeg_);
	XMVECTOR dir = XMVector3Normalize(XMVectorSet(cosf(pitchRad) * sinf(yawRad), sinf(pitchRad), cosf(pitchRad) * cosf(yawRad), 0.0f));

	//2. 카메라 위치 = 타겟 위치 + 방향 * 거리
	XMVECTOR focus = XMLoadFloat3(&target_);
	XMVECTOR cameraPos = XMVectorAdd(focus, dir * distance_);
	XMVECTOR up = UP;

	//3. 카메라 적용
	camera_->setPositionXM(cameraPos);
	camera_->setTargetXM(focus);
	camera_->setUpDirectionXM(up);
}
