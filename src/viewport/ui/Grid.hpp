#pragma once
#include "render/RenderObject.hpp"
#include "render/tools/GridMesh.hpp"
#include "common/Math.hpp"

namespace Render 
{
	namespace Tools 
	{
		class Grid : public RenderObject
		{
		public:
			virtual ~Grid() = default;

			bool initialize(ID3D11Device* device, int size, float spacing);

			void setGridSize(int size);
			void setGridSpacing(float spacing);
			int getGridSize() const { return gridSize_; }
			float getGridSpacing() const { return gridSpacing_; }


		private:
			int gridSize_ = 10;
			float gridSpacing_ = 1.0f;
			ID3D11Device* device_ = nullptr;

			void rebuildMesh();
		};
	}
}
