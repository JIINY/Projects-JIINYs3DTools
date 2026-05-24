#pragma once
#include "render/RenderObject.hpp"
#include "render/tools/GridMesh.hpp"
#include "common/Math.hpp"

class MaterialManager;
namespace Render 
{ 
	class RenderCommandQueue;
	class PixelShader; 
}


namespace Render::Tools
{
	struct GridContext
	{
		ID3D11Device* device = nullptr;
		MaterialManager* matManager = nullptr;
		int size = 10;
		float spacing = 1.0f;
	};

	class Grid : public Render::RenderObject
	{
	public:
		virtual ~Grid() = default;

		bool initialize(GridContext context);

		void addToRenderQueue(Render::RenderCommandQueue* queue, const DirectX::XMMATRIX& viewMat);
		void setGridSize(int size);
		void setGridSpacing(float spacing);
		int getGridSize() const { return size_; }
		float getGridSpacing() const { return spacing_; }


	private:
		int size_ = 10;
		float spacing_ = 1.0f;
		ID3D11Device* device_ = nullptr;
		MaterialManager* materialManager_ = nullptr;

		void rebuildMesh();
	};
}
