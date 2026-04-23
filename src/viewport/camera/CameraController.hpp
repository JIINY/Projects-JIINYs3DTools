#pragma once
#include <string>
#include <DirectXMath.h>
#include "common/Math.hpp"
#include "event/editorEvent/EditorEventSubscriber.hpp"

class Camera;

class CameraController 
{
public:
	virtual ~CameraController() = default;

	struct CameraCoreState
	{
		Math::Vec3 position;
		Math::Vec3 target;
		Math::Vec3 up;
		float distance;
	};

	virtual void update(float deltaTime) = 0;
	virtual void onActivate(const CameraController* prevController, CameraMode prevMode) {}
	virtual void onDeactivate() {}

	virtual CameraCoreState getCoreState() const 
	{
		CameraCoreState empty = {};
		return empty;
	};
	virtual Math::Vec3 getTarget() const { return { 0.0f, 0.0f, 0.0f }; }
	virtual float getDistance() const { return 10.0f; }
	virtual float getPitch() const { return 0.0f; }
	virtual float getYaw() const { return 0.0f; }

	void setCamera(Camera* cam) { camera_ = cam; }
	Camera* getCamera() const { return camera_; }



protected:
	std::vector<EditorEventSubscriptionID> editorEventSubID_;
	Camera* camera_ = nullptr;
	using ViewHandlerFunc = void (CameraController::*)();
};
