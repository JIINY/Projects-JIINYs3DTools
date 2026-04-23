#pragma once
#include "Camera.hpp"
#include <DirectXMath.h>
#include "common/Math.hpp"
using namespace Math;
using namespace DirectX;


void Camera::setPosition(const Vec3& pos) 
{
	position_ = pos;
	viewDirty_ = true;
	viewProjDirty_ = true;
}

void Camera::setTarget(const Vec3& target) 
{
	target_ = target;
	viewDirty_ = true;
	viewProjDirty_ = true;
}

void Camera::setUpDirection(const Vec3& up) 
{
	up_ = up;
	viewDirty_ = true;
	viewProjDirty_ = true;
}

void Camera::setPositionXM(const XMVECTOR pos) 
{
	XMStoreFloat3(&position_, pos);
	viewDirty_ = true;
	viewProjDirty_ = true;
}

void Camera::setTargetXM(const XMVECTOR target) 
{
	XMStoreFloat3(&target_, target);
	viewDirty_ = true;
	viewProjDirty_ = true;
}

void Camera::setUpDirectionXM(const XMVECTOR up) 
{
	XMStoreFloat3(&up_, up);
	viewDirty_ = true;
	viewProjDirty_ = true;
}



void Camera::setProjection(float fovY, float aspect, float nearZ, float farZ) 
{
	fovY_ = fovY;
	aspect_ = aspect;
	nearZ_ = nearZ;
	farZ_ = farZ;
	viewProjDirty_ = true;
}

void Camera::setFovY(const float& fovY) 
{
	fovY_ = fovY;
	projDirty_ = true;
	viewProjDirty_ = true;
}

void Camera::setAspect(const float& aspect) 
{
	aspect_ = aspect;
	projDirty_ = true;
	viewProjDirty_ = true;
}

void Camera::setNearZ(const float& nearZ) 
{
	if (nearZ <= 0.0f) { nearZ_ = 0.01f; }
	else { nearZ_ = nearZ; }

	projDirty_ = true;
	viewProjDirty_ = true;
}

void Camera::setFarZ(const float& farZ) 
{
	farZ_ = farZ;
	projDirty_ = true;
	viewProjDirty_ = true;
}


Mat4 Camera::getViewMatrix() const 
{
	cachedView_ = this->getViewMatrixXM();
	Mat4 mat;
	XMStoreFloat4x4(&mat, cachedView_);
	return mat;
}

Mat4 Camera::getProjectionMatrix() const
{
	if (projDirty_)
	{
		cachedProj_ = XMMatrixPerspectiveFovLH(fovY_, aspect_, nearZ_, farZ_);
		projDirty_ = false;
	}
	Mat4 mat;
	XMStoreFloat4x4(&mat, cachedProj_);
	return mat;
}

Mat4 Camera::getViewProjectionMatrix() const 
{
	if (viewProjDirty_)
	{
		cachedView_ = this->getViewMatrixXM();
		if (projDirty_)
		{
			cachedProj_ = XMMatrixPerspectiveFovLH(fovY_, aspect_, nearZ_, farZ_);
			projDirty_ = false;
		}

		cachedViewProj_ = cachedView_ * cachedProj_;
		viewProjDirty_ = false;
	}
	Mat4 mat;
	XMStoreFloat4x4(&mat, cachedViewProj_);
	return mat;
}

const DirectX::XMMATRIX& Camera::getViewMatrixXM() const 
{
	if (viewDirty_) 
	{
		cachedView_ = XMMatrixLookAtLH(
			XMLoadFloat3((XMFLOAT3*)&position_),
			XMLoadFloat3((XMFLOAT3*)&target_),
			XMLoadFloat3((XMFLOAT3*)&up_));

		viewDirty_ = false;
		viewProjDirty_ = true;
	}
	return cachedView_;
}

const DirectX::XMMATRIX& Camera::getProjectionMatrixXM() const 
{
	if (projDirty_) 
	{
		cachedProj_ = XMMatrixPerspectiveFovLH(fovY_, aspect_, nearZ_, farZ_);
		projDirty_ = false;
		viewProjDirty_ = true;
	}
	return cachedProj_;
}

const DirectX::XMMATRIX& Camera::getViewProjectionMatrixXM() const 
{
	getViewMatrixXM();
	getProjectionMatrixXM();

	if (viewProjDirty_) 
	{
		cachedViewProj_ = XMMatrixMultiply(cachedView_, cachedProj_);
		viewProjDirty_ = false;
	}
	return cachedViewProj_;
}

Math::Ray Camera::convertScreenPointToRay(float screenX, float screenY, float viewportX, float viewportY) const
{
	Ray emptyRay;
	emptyRay.origin = Vec3(0, 0, 0);
	emptyRay.direction = Vec3(0, 0, 0);
	if (screenX < 0.0f || screenX > viewportX || screenY < 0.0f || screenY > viewportY)
	{
		return emptyRay;
	}

	//행렬 준비
	XMMATRIX projMat = getProjectionMatrixXM();
	XMMATRIX viewMat = getViewMatrixXM();
	XMMATRIX worldMat = XMMatrixIdentity();

	XMVECTOR nearSrc = XMVectorSet(screenX, screenY, 0.0f, 1.0f);
	XMVECTOR farSrc = XMVectorSet(screenX, screenY, 1.0f, 1.0f);

	XMVECTOR nearPoint = XMVector3Unproject(nearSrc, 0.0f, 0.0f, viewportX, viewportY, 0.0f, 1.0f, projMat, viewMat, worldMat);
	XMVECTOR farPoint = XMVector3Unproject(farSrc, 0.0f, 0.0f, viewportX, viewportY, 0.0f, 1.0f, projMat, viewMat, worldMat);

	XMVECTOR dir = farPoint - nearPoint;
	if (XMVectorGetX(XMVector3LengthSq(dir)) < 0.0001f) { return emptyRay; }
	dir = XMVector3Normalize(dir);

	//정규화 체크
	float checkLength = XMVectorGetX(XMVector3Length(dir));
	if (abs(checkLength - 1.0f) > 0.01f) { return emptyRay;	}

	Ray ray;
	XMStoreFloat3(&ray.origin, nearPoint);
	XMStoreFloat3(&ray.direction, dir);

	return ray;
}
