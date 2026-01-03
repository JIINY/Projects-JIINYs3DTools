#pragma once
#include <DirectXMath.h>
#include <utility>
#include "Mode.hpp"

namespace Math 
{
	using Vec2 = DirectX::XMFLOAT2;
	using Vec3 = DirectX::XMFLOAT3;
	using Vec4 = DirectX::XMFLOAT4;
	using Mat4 = DirectX::XMFLOAT4X4;

	extern const DirectX::XMVECTOR FORWARD;
	extern const DirectX::XMVECTOR BACKWARD;
	extern const DirectX::XMVECTOR LEFT;
	extern const DirectX::XMVECTOR RIGHT;
	extern const DirectX::XMVECTOR UP;
	extern const DirectX::XMVECTOR BOTTOM;

	inline Vec3 operator+(const Vec3& a, const Vec3& b)
	{
		DirectX::XMVECTOR va = DirectX::XMLoadFloat3(&a);
		DirectX::XMVECTOR vb = DirectX::XMLoadFloat3(&b);
		DirectX::XMVECTOR vr = DirectX::XMVectorAdd(va, vb);
		Vec3 result{};
		DirectX::XMStoreFloat3(&result, vr);
		return result;
	}

	Vec3 Normalize(const Vec3& v);
	float Length(const Vec3& v);
	std::pair<float, float> DirectionToPitchYaw(const Vec3& dir);
	Vec3 PitchYawToDirection(float pitch, float yaw);

	float getPitchFromView(ViewMode view);
	float getYawFromView(ViewMode view);
}
