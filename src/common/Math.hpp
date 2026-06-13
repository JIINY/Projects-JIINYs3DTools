#pragma once
#include <cmath>
#include <algorithm>
#include <DirectXMath.h>
#include <utility>
#include "Mode.hpp"


inline DirectX::XMFLOAT2 operator+(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b)
{
	return { a.x + b.x, a.y + b.y };
}
inline DirectX::XMFLOAT2& operator+=(DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b)
{
	a.x += b.x; a.y += b.y;
	return a;
}
inline DirectX::XMFLOAT2 operator-(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b)
{
	return { a.x - b.x, a.y - b.y };
}
inline DirectX::XMFLOAT2& operator-=(DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b)
{
	a.x -= b.x; a.y -= b.y;
	return a;
}
inline DirectX::XMFLOAT2 operator*(const DirectX::XMFLOAT2& a, float b)
{
	return { a.x * b, a.y * b };
}
inline DirectX::XMFLOAT2& operator*=(DirectX::XMFLOAT2& a, float b)
{
	a.x *= b; a.y *= b;
	return a;
}
inline DirectX::XMFLOAT2 operator/(const DirectX::XMFLOAT2& a, float b)
{
	float invB = 1.0f / b;
	return { a.x * invB, a.y * invB };
}
inline DirectX::XMFLOAT2& operator/=(DirectX::XMFLOAT2& a, float b)
{
	float invB = 1.0f / b;
	a.x *= invB; a.y *= invB;
	return a;
}


inline DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}
inline DirectX::XMFLOAT3& operator+=(DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b) 
{
	a.x += b.x; a.y += b.y; a.z += b.z;
	return a;
}
inline DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}
inline DirectX::XMFLOAT3& operator-=(DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	a.x -= b.x; a.y -= b.y; a.z -= b.z;
	return a;
}
inline DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& a, float b) 
{
	return { a.x * b, a.y * b, a.z * b };
}
inline DirectX::XMFLOAT3& operator*=(DirectX::XMFLOAT3& a, float b) 
{
	a.x *= b; a.y *= b; a.z *= b;
	return a;
}
inline DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& a, float b) 
{
	float invB = 1.0f / b;
	return { a.x * invB, a.y * invB, a.z * invB };
}
inline DirectX::XMFLOAT3& operator/=(DirectX::XMFLOAT3& a, float b) 
{
	float invB = 1.0f / b;
	a.x *= invB; a.y *= invB; a.z *= invB;
	return a;
}


namespace Math 
{
	constexpr float PI = 3.14159265359f;
	constexpr float TWO_PI = 6.28318530718f;
	constexpr float HALF_PI = 1.57079632679f;

	using Vec2 = DirectX::XMFLOAT2;
	using Vec3 = DirectX::XMFLOAT3;
	using Vec4 = DirectX::XMFLOAT4;
	using Mat4 = DirectX::XMFLOAT4X4;
	using Int2 = DirectX::XMINT2;

	extern const DirectX::XMVECTOR FORWARD;
	extern const DirectX::XMVECTOR BACKWARD;
	extern const DirectX::XMVECTOR LEFT;
	extern const DirectX::XMVECTOR RIGHT;
	extern const DirectX::XMVECTOR UP;
	extern const DirectX::XMVECTOR BOTTOM;

	Vec3 normalize(const Vec3& v);
	float length(const Vec3& v);
	std::pair<float, float> directionToPitchYaw(const Vec3& dir);
	Vec3 pitchYawToDirection(float pitch, float yaw);

	float getPitchFromView(ViewMode view);
	float getYawFromView(ViewMode view);

	double roundFloat(float val, int digits = 3);
	int floatToColor255(float val);
	float color255ToFloat(int val);

	//구면 좌표계 → 직교 좌표계 변환
	inline Vec3 getSphericalCoord(float radius, float theta, float phi)
	{
		float x = radius * sinf(phi) * cosf(theta);
		float y = radius * cosf(phi);
		float z = radius * sinf(phi) * sinf(theta);
		return Vec3(x, y, z);
	}

	struct Ray
	{
		Math::Vec3 origin;
		Math::Vec3 direction;
	};

	Ray transformRay(const Ray& ray, const DirectX::XMMATRIX& transform);

	bool castRayOnPlane(const Ray& ray, const Vec3& planePoint, const Vec3& planeNormal, Vec3& outHitPoint);

	enum class Axis
	{
		X,
		Y,
		Z,
		Hover,
		Locked,
		Default,
		Count
	};

	struct AxisInfo 
	{
		static const Vec4 colorX;
		static const Vec4 colorY;
		static const Vec4 colorZ;
		static const Vec4 colorHover;
		static const Vec4 colorLocked;
		static const Vec4 colorDefault;

		static const DirectX::XMMATRIX rotationX;
		static const DirectX::XMMATRIX rotationY;
		static const DirectX::XMMATRIX rotationZ;

		static const Vec4& GetColor(Axis axis) 
		{
			switch (axis) 
			{
			case Axis::X: return colorX;
			case Axis::Y: return colorY;
			case Axis::Z: return colorZ;
			case Axis::Hover: return colorHover;
			case Axis::Locked: return colorLocked;
			case Axis::Default:
			case Axis::Count:
			default: return colorDefault;
			}
		}

		static const DirectX::XMMATRIX& GetRotationMX(Axis axis)
		{
			switch (axis) 
			{
			case Axis::X: return rotationX;
			case Axis::Y: return rotationY;
			case Axis::Z: return rotationZ;
			default: return rotationY;
			}
		}
	};
}
