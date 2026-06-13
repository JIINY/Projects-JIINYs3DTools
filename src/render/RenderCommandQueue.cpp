#include "RenderCommandQueue.hpp"
#include <vector>
#include <algorithm>
#include <cassert>
#include "render/IRenderable.hpp"
#include "Material.hpp"

#include "common/DebugLog.hpp"
using namespace std;


namespace Render 
{
	void RenderCommandQueue::addCommand(Render::IRenderable* obj, float depth)
	{
		if (!obj) { return; }

		Render::RenderCommand cmd;
		cmd.renderable_ = obj;
		cmd.sortKey_ = calculateSortKey(obj->getRenderQueue(), depth);

		queue_.push_back(cmd);
	}

	void RenderCommandQueue::addCommand(Render::IRenderable* obj, float depth, OverridePSType psType)
	{
		if (!obj) { return; }

		Render::RenderCommand cmd;
		cmd.renderable_ = obj;
		cmd.psType_ = psType;
		cmd.customPS_ = nullptr;
		cmd.sortKey_ = calculateSortKey(obj->getRenderQueue(), depth);

		queue_.push_back(cmd);
	}

	void RenderCommandQueue::addCommand(Render::IRenderable* obj, float depth, shared_ptr<PixelShader> customPS)
	{
		if (!obj) { return; }

		Render::RenderCommand cmd;
		cmd.renderable_ = obj;
		cmd.psType_ = OverridePSType::Custom;
		cmd.customPS_ = customPS;
		cmd.sortKey_ = calculateSortKey(obj->getRenderQueue(), depth);

		queue_.push_back(cmd);
	}

	uint64_t RenderCommandQueue::calculateSortKey(int renderQueue, float depth)
	{
		//상위 16bit: RenderQueue, 16bit: Layer or Material, 하위32bit: 거리(불투명-투명)
		uint64_t queueID = static_cast<uint64_t>(renderQueue);
		uint64_t depthKey = static_cast<uint64_t>(depth * 100.0f);

		//불투명: 가까운 것부터 SortKey 작게, 투명: 먼 것부터 SortKey 작게
		if (renderQueue >= static_cast<int>(RenderQueue::Transparent))
		{
			depthKey = ~depthKey; //비트 반전
		}

		return (queueID << 32) | (depthKey & 0xFFFFFFFF); //QueueID -> 거리순으로 단순화하여 일단 구현
	}

	void RenderCommandQueue::sort()
	{
		for (const auto& cmd : queue_)
		{
			int renderQueue = (int)(cmd.sortKey_ >> 32);
		}

		std::sort(queue_.begin(), queue_.end(),
			[](const RenderCommand& a, const RenderCommand& b)
			{
				//키가 작은 순서대로 정렬
				return a.sortKey_ < b.sortKey_;
			});
	}

	void RenderCommandQueue::execute(ID3D11DeviceContext* context, const Math::Mat4& view, const Math::Mat4& proj)
	{
		for (const Render::RenderCommand& cmd : queue_)
		{
			shared_ptr<PixelShader> ps = nullptr;

			switch (cmd.psType_)
			{
			case OverridePSType::Black:
			{
				ps = Render::Material::getWireBlackPS();
				break;
			}
			case OverridePSType::Red:
			{
				ps = Render::Material::getWireRedPS();
				break;
			}
			case OverridePSType::Collider:
			{
				ps = Render::Material::getColliderPS();
				break;
			}
			case OverridePSType::Custom:
			{
				ps = cmd.customPS_;
				break;
			}
			case OverridePSType::None:
			default:
				ps = nullptr;
				break;
			}

			cmd.renderable_->draw(context, view, proj, ps);
		}
	}
}
