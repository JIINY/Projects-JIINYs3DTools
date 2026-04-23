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
#include "event/editorEvent/EditorEventSubscriber.hpp"
#include "event/editorEvent/io/MouseEvent.hpp"
using namespace std;
using namespace Math;
using namespace DirectX;


FixedAngleCameraController::FixedAngleCameraController(ViewMode view) : FixedAngleCameraController(getPitchFromView(view), getYawFromView(view)) {}
FixedAngleCameraController::FixedAngleCameraController(float pitchDeg, float yawDeg) : pitchDeg_(pitchDeg), yawDeg_(yawDeg) {}

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
	//이벤트 구독
	auto mouseDown = EditorEventSubscriber::get().subscribe<MouseDownEditorEvent>([this](const MouseDownEditorEvent& event)
		{
			this->onMouseDowned(event);
		});
	editorEventSubID_.push_back(mouseDown);

	auto mouseUp = EditorEventSubscriber::get().subscribe<MouseUpEditorEvent>([this](const MouseUpEditorEvent& event)
		{
			this->onMouseUpped(event);
		});
	editorEventSubID_.push_back(mouseUp);

	auto mouseWheel = EditorEventSubscriber::get().subscribe<MouseWheelEditorEvent>([this](const MouseWheelEditorEvent& event)
		{
			this->onMouseWheeled(event);
		});
	editorEventSubID_.push_back(mouseWheel);

	auto mouseMove = EditorEventSubscriber::get().subscribe<MouseMoveEditorEvent>([this](const MouseMoveEditorEvent& event)
		{
			this->onMouseMoved(event);
		});
	editorEventSubID_.push_back(mouseMove);


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

void FixedAngleCameraController::onDeactivate()
{
	for (auto id : editorEventSubID_)
	{
		EditorEventSubscriber::get().unsubscribe(id);
	}
	editorEventSubID_.clear();
}

void FixedAngleCameraController::onMouseDowned(const MouseDownEditorEvent& event)
{
	switch (event.button_)
	{
	case 1: 
	{
		dragging_ = true;
		lastMousePos_ = event.pos_;
	}
	case 2:
	{
		wheelDragging_ = true;
		lastMousePos_ = event.pos_;
	}
	default: break;
	}
}

void FixedAngleCameraController::onMouseUpped(const MouseUpEditorEvent& event)
{
	switch (event.button_)
	{
	case 1: dragging_ = false; break;
	case 2: wheelDragging_ = false; break;
	default: break;
	}
}

void FixedAngleCameraController::onMouseMoved(const MouseMoveEditorEvent& event)
{
	if (dragging_)
	{
		int dx = event.pos_.x - lastMousePos_.x;
		int dy = event.pos_.y - lastMousePos_.y;
		lastMousePos_ = event.pos_;

		float pitchRad = XMConvertToRadians(pitchDeg_);
		float yawRad = XMConvertToRadians(yawDeg_);

		XMVECTOR forward = XMVectorSet(cosf(pitchRad) * sinf(yawRad), sinf(pitchRad), cosf(pitchRad) * cosf(yawRad), 0.0f);
		forward = XMVector3Normalize(forward);

		XMVECTOR right = XMVector3Cross(UP, forward);
		right = XMVector3Normalize(right);

		XMVECTOR camUp = XMVector3Cross(forward, right);
		camUp = XMVector3Normalize(camUp);

		float speed = panSpeed_ * distance_;
		XMVECTOR translation = (right * (float)dx * speed) + (camUp * (float)dy * speed);
		XMVECTOR currentTarget = XMLoadFloat3(&target_);

		currentTarget = XMVectorAdd(currentTarget, translation);
		XMStoreFloat3(&target_, currentTarget);
		updateView();
	}
	else if (wheelDragging_)
	{
		int dx = fabs(event.pos_.x - lastMousePos_.x);
		int dy = fabs(event.pos_.y - lastMousePos_.y);
	
		if (dx > 2 || dy > 2)
		{
			AppEventPublisher::get().publish(CameraModeChangedEvent(CameraMode::FreeArm, ViewMode::None));
		}
	}
}

void FixedAngleCameraController::onMouseWheeled(const MouseWheelEditorEvent& event)
{
	distance_ -= event.delta_ * zoomSpeed_ * distance_ * 0.1f;
	if (distance_ < 0.1f) { distance_ = 0.1f; }
	updateView();
}
