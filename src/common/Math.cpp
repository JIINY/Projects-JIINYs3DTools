#include "Math.hpp"
#include <cmath>
#include <DirectXMath.h>
using namespace std;
using namespace DirectX;

namespace Math 
{
	const XMVECTOR FORWARD = XMVectorSet(0, 0, -1, 0);
	const XMVECTOR BACKWARD = XMVectorSet(0, 0, 1, 0);
	const XMVECTOR LEFT = XMVectorSet(1, 0, 0, 0);
	const XMVECTOR RIGHT = XMVectorSet(-1, 0, 0, 0);
	const XMVECTOR UP = XMVectorSet(0, 1, 0, 0);
	const XMVECTOR BOTTOM = XMVectorSet(0, -1, 0, 0);

	Vec3 normalize(const Vec3& v)
	{
		XMVECTOR vector = XMLoadFloat3(&v);
		XMVECTOR normalized = XMVector3Normalize(vector);
		Vec3 result;
		XMStoreFloat3(&result, normalized);
		return result;
	}

	float length(const Vec3& v) 
	{
		XMVECTOR vector = XMLoadFloat3(&v);
		XMVECTOR lengthVector = XMVector3Length(vector);
		return XMVectorGetX(lengthVector);
	}

	Vec3 PitchYawToDirection(float pitch, float yaw) 
	{
		float x = cos(pitch) * sin(yaw);
		float y = sin(pitch);
		float z = cos(pitch) * cos(yaw);
		return normalize(Vec3(x, y, z));
	}

	pair<float, float> DirectionToPitchYaw(const Vec3& dir) 
	{
		Vec3 n = normalize(dir);
		float pitch = asin(n.y); //sin(pitch) = y
		float yaw = atan2(n.x, n.z); //tan(yaw) = x / z
		return { pitch, yaw };
	}

	float getPitchFromView(ViewMode view) 
	{
		switch (view) {
		case ViewMode::TopView: return -90.0f;
		case ViewMode::BottomView: return 90.0f;
		default: return 0.0f;
		}
	}

	float getYawFromView(ViewMode view) 
	{
		switch (view) {
		case ViewMode::FrontView: return 180.0f;
		case ViewMode::BackView: return 0.0f;
		case ViewMode::LeftView: return -90.0f;
		case ViewMode::RightView: return 90.0f;
		default: return 0.0f;
		}
	}
}
