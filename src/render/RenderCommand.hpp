#pragma once
#include <cstdint>

enum class RenderCommandType 
{
	Grid,
	WorldAxes,
	CameraModeUI
};

struct RenderCommand 
{
	int layer_ = 0; //0=Scene, 100=Overlay, 1000=UI
	std::uint64_t sortKey_ = 0; //동일 레이어 내에서 정렬
	RenderCommandType type_{};
	const void* drawData_ = nullptr; //프레임 한정 데이터
};
