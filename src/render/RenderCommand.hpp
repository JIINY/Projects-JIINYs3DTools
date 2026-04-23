#pragma once
#include <cstdint>
#include <memory>
#include "render/PixelShader.hpp"


namespace Render
{
	class IRenderable;

	enum class OverridePSType
	{
		None,
		Black,
		Red,
		Custom
	};

	struct RenderCommand
	{
		std::uint64_t sortKey_ = 0;
		Render::IRenderable* renderable_ = nullptr;
		OverridePSType psType_ = OverridePSType::None;
		std::shared_ptr<PixelShader> customPS_ = nullptr;
	};

	enum class RenderQueue : int 
	{
		Background = 1000,
		Geometry = 2000,
		AlphaTest = 2450,
		Transparent = 3000,
		Overlay = 4000
	};
}
