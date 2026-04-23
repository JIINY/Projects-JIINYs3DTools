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

void TargetCameraController::onDeactivate()
{
	for (auto id : editorEventSubID_)
	{
		EditorEventSubscriber::get().unsubscribe(id);
	}
	editorEventSubID_.clear();
}

void TargetCameraController::onMouseDowned(const MouseDownEditorEvent& event)
{
	if (event.button_ == 1)
	{
		dragging_ = true;
		lastMousePos_ = event.pos_;
	}
}

void TargetCameraController::onMouseUpped(const MouseUpEditorEvent& event)
{
	if (event.button_ == 1)
	{
		dragging_ = false;
	}
}

void TargetCameraController::onMouseMoved(const MouseMoveEditorEvent& event)
{
	if (dragging_)
	{
		int dx = event.pos_.x - lastMousePos_.x;
		int dy = event.pos_.y - lastMousePos_.y;
		lastMousePos_ = event.pos_;

		yawDeg_ += dx * rotationSpeed_;
		pitchDeg_ -= dy * rotationSpeed_;

		if (pitchDeg_ < minPitch_) pitchDeg_ = minPitch_;
		if (pitchDeg_ > maxPitch_) pitchDeg_ = maxPitch_;
		updateView();
	}
}

void TargetCameraController::onMouseWheeled(const MouseWheelEditorEvent& event)
{
	distance_ -= event.delta_ * zoomSpeed_;

	if (distance_ < minDistance_) { distance_ = minDistance_; }
	if (distance_ > maxDistance_) { distance_ = maxDistance_; }
	updateView();
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
