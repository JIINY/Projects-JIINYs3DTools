#pragma once
#include <array>
#include <string>

enum class AppMode //추가시 syncModeState에서 MainMenuBar 설정은 직접 추가 필요 (+ UIElementID)
{
	Edit = 0,
	Play = 1,
	Count
};

inline constexpr std::array<const char*, 2> AppModeName =
{
	"EDIT",
	"PLAY"
};


enum class CameraMode
{
	FreeArm = 0, //거치대+암 구조
	Target = 1, //Focus 중심 회전
	FreeFly = 2, //FPS카메라처럼 자유 이동
	FixedAngle = 3,
	Count
};

enum class ViewMode
{
	None = 0,
	TopView = 1,
	FrontView = 2,
	LeftView = 3,
	RightView = 4,
	BackView = 5,
	BottomView = 6,
	Count
};

enum class CameraUIMode //UI에서 편하게 다루기 위해 CameraMode + ViewMode
{
	FreeArm = 0,
	Target = 1,
	FreeFly = 2,
	FixedAngle = 3,
	TopView = 4,
	FrontView = 5,
	LeftView = 6, 
	RightView = 7,
	BackView = 8,
	BottomView = 9,
	Count
};

enum class TransformMode
{
	Translate,
	Rotate,
	Scale,
	Count
};
