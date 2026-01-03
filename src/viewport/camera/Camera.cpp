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
