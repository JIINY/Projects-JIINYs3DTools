#include "Grid.hpp"
#include <cmath>
#include <cassert>
#include "core/manager/resources/MaterialManager.hpp"
#include "render/tools/GridMesh.hpp"
#include "render/RenderCommandQueue.hpp"
#include "../shaders/Unlit_VertexColor/VertexColorMaterial.hpp"

#include "common/DebugLog.hpp"
using namespace std;
using namespace Render;
using namespace DirectX;


namespace Render::Tools
{
	bool Grid::initialize(GridContext context)
	{
		device_ = context.device;
		materialManager_ = context.matManager;
		size_ = context.size;
		spacing_ = context.spacing;

		//GridMesh 생성
		auto mesh = make_shared<GridMesh>();
		mesh->rebuild(device_, size_, spacing_);

		//매티리얼 생성
		auto material = materialManager_->createMaterial<Materials::VertexColorMaterial>();

		//부모(RenderObject)에 조립 요청
		RenderObject::initialize(device_, mesh, material);

		return true;
	}

	void Grid::addToRenderQueue(RenderCommandQueue* queue, const XMMATRIX& viewMat)
	{
		if (!queue) { return; }

		XMMATRIX invView = XMMatrixInverse(nullptr, viewMat);
		XMVECTOR camPos = invView.r[3];

		XMMATRIX world = XMLoadFloat4x4(&this->getWorldMatForShader());
		XMVECTOR gridPos = world.r[3];
		XMVECTOR diff = XMVectorSubtract(gridPos, camPos);
		float depth = XMVectorGetX(XMVector3Length(diff));

		queue->addCommand(this, depth);
	}

	void Grid::setGridSize(int size) 
	{
		if (size_ == size) { return; }
		size_ = size;
		rebuildMesh();
	}

	void Grid::setGridSpacing(float spacing) 
	{
		if (abs(spacing_ - spacing) < 1e-5f) { return; }
		spacing_ = spacing;
		rebuildMesh();
	}

	void Grid::rebuildMesh() 
	{
		if (!device_) { return; }

		auto currentMesh = getMesh();
		auto gridMesh = static_pointer_cast<GridMesh>(currentMesh);

		if (gridMesh) 
		{
			gridMesh->rebuild(device_, size_, spacing_);
		}
	}
}
