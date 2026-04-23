#pragma once
#include <vector>
#include <algorithm>
#include "RenderCommand.hpp"
#include "render/IRenderable.hpp"
#include "render/PixelShader.hpp"


namespace Render
{
	class RenderCommandQueue {
	public:
		RenderCommandQueue() { queue_.reserve(INITIAL_CAPACITY); }
		RenderCommandQueue(const RenderCommandQueue&) = delete;
		RenderCommandQueue& operator=(const RenderCommandQueue&) = delete;

		void addCommand(Render::IRenderable* obj, float depth);
		void addCommand(Render::IRenderable* obj, float depth, OverridePSType psType);
		void addCommand(Render::IRenderable* obj, float depth, std::shared_ptr<PixelShader> customPS);

		void sort();
		void execute(ID3D11DeviceContext* context, const Math::Mat4& view, const Math::Mat4& proj);

		void clear() { queue_.clear(); }
		bool isEmpty() const { return queue_.empty(); }
		std::size_t getSize() const { return queue_.size(); }

		const std::vector<Render::RenderCommand>& getQueue() const { return queue_; }

	private:
		std::vector<Render::RenderCommand> queue_;
		static constexpr size_t INITIAL_CAPACITY = 1000;

		uint64_t calculateSortKey(int renderQueue, float depth);
	};
}
