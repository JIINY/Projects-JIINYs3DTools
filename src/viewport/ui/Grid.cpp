#include "Grid.hpp"
#include <cmath>
#include <cassert>
#include "render/tools/GridMesh.hpp"
#include "../shaders/Unlit_VertexColor/VertexColorMaterial.hpp"
using namespace std;
using namespace Render;


namespace Render 
{
	namespace Tools 
	{
		bool Grid::initialize(ID3D11Device* device, int size, float spacing)
		{
			device_ = device;
			gridSize_ = size;
			gridSpacing_ = spacing;

			//GridMesh 생성
			auto mesh = make_shared<GridMesh>();
			mesh->rebuild(device, size, spacing);

			//매티리얼 생성
			auto material = make_shared<Materials::VertexColorMaterial>();
			material->initialize(device,
				L"shaders/Unlit_VertexColor/Unlit_VS_VertexColor.hlsl",
				L"shaders/Unlit_VertexColor/Unlit_PS_VertexColor.hlsl");

			//부모(RenderObject)에 조립 요청
			RenderObject::initialize(device, mesh, material);

			return true;
		}

		void Grid::setGridSize(int size) 
		{
			if (gridSize_ == size) { return; }
			gridSize_ = size;
			rebuildMesh();
		}

		void Grid::setGridSpacing(float spacing) 
		{
			if (abs(gridSpacing_ - spacing) < 1e-5f) { return; }
			gridSpacing_ = spacing;
			rebuildMesh();
		}

		void Grid::rebuildMesh() 
		{
			if (!device_) { return; }

			auto currentMesh = getMesh();
			auto gridMesh = static_pointer_cast<GridMesh>(currentMesh);

			if (gridMesh) 
			{
				gridMesh->rebuild(device_, gridSize_, gridSpacing_);
			}
		}
	}
}
