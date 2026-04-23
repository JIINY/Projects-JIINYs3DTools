#pragma once
#include <DirectXMath.h>
#include "common/Math.hpp"


class Camera {
public:
	void setPosition(const Math::Vec3& pos);
	void setTarget(const Math::Vec3& target);
	void setUpDirection(const Math::Vec3& up); //X는 Y와 Z의 수직관계에 따라 자동 결정된다.
	void setPositionXM(const DirectX::XMVECTOR pos);
	void setTargetXM(const DirectX::XMVECTOR target);
	void setUpDirectionXM(const DirectX::XMVECTOR up);

	const Math::Vec3& getPosition() const { return position_; }
	const Math::Vec3& getTarget() const { return target_; }
	const Math::Vec3& getUpDirection() const { return up_; }
	const DirectX::XMVECTOR getPositionXM() const { return DirectX::XMLoadFloat3(&position_); }
	const DirectX::XMVECTOR getTargetXM() const { return DirectX::XMLoadFloat3(&target_); }
	const DirectX::XMVECTOR getUpDirectionXM() const { return DirectX::XMLoadFloat3(&up_); }

	
	void setProjection(float fovY, float aspect, float nearZ, float farZ);

	Math::Mat4 getViewMatrix() const;
	Math::Mat4 getProjectionMatrix() const;
	Math::Mat4 getViewProjectionMatrix() const;

	const DirectX::XMMATRIX& getViewMatrixXM() const;
	const DirectX::XMMATRIX& getProjectionMatrixXM() const;
	const DirectX::XMMATRIX& getViewProjectionMatrixXM() const;

	void setFovY(const float& fovY);
	void setAspect(const float& aspect);
	void setNearZ(const float& nearZ);
	void setFarZ(const float& farZ);

	const float& getFovY() const { return fovY_; }
	const float& getAspect() const { return aspect_; }
	const float& getNearZ() const { return nearZ_; }
	const float& getFarZ() const { return farZ_; }

	void setWidth(int width) { width_ = width; }
	void setHeight(int height) { height_ = height; }
	int getWidth() const { return width_; }
	int getHeight() const { return height_; }

	Math::Ray convertScreenPointToRay(float screenX, float screenY, float viewportX, float viewportY) const;

private:
	Math::Vec3 position_ = { 0, 0, -5 };
	Math::Vec3 target_ = { 0, 0, 0 };
	Math::Vec3 up_ = { 0, 1, 0 };

	float fovY_ = DirectX::XM_PIDIV4; //45도
	float aspect_ = 1.0f;
	float nearZ_ = 0.1f;
	float farZ_ = 100.0f;

	int width_ = 1280;
	int height_ = 800;

	mutable DirectX::XMMATRIX cachedView_ = DirectX::XMMatrixIdentity();
	mutable DirectX::XMMATRIX cachedProj_ = DirectX::XMMatrixIdentity();
	mutable DirectX::XMMATRIX cachedViewProj_ = DirectX::XMMatrixIdentity();

	mutable bool viewDirty_ = true;
	mutable bool projDirty_ = true;
	mutable bool viewProjDirty_ = true; //dirty: 더이상 유효하지 않음, 다시 계산이 필요함
};
