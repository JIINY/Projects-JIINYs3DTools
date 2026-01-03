#include "ToolObjectManager.hpp"
#include <vector>
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>
#include "renderer/Renderer.hpp"
#include "render/RenderObject.hpp"
#include "viewport/ui/Grid.hpp"
using namespace std;
using namespace Render;


ToolObjectManager* ToolObjectManager::get()
{
    static ToolObjectManager instance;
    return &instance;
}

void ToolObjectManager::initialize(Renderer* renderer)
{
	assert(renderer && "[에러] BaseObjectManager 초기화 실패: 렌더러가 null입니다.");
	if (isInitialized_) { return; }

	device_ = renderer->getDevice();
	assert(device_ && "[에러] BaseObjectManager 초기화 실패: 디바이스를 가져올 수 없습니다.");
	context_ = renderer->getDeviceContext();
	assert(context_ && "[에러] BaseObjectManager 초기화 실패: 컨텍스트를 가져올 수 없습니다.");

	isInitialized_ = true;
}

void ToolObjectManager::draw(Renderer* renderer, const DirectX::XMMATRIX& viewMat, const DirectX::XMMATRIX& projMat)
{
	assert(isInitialized_ && "[에러] BaseObjectManager가 초기화되지 않았습니다.");
	assert(renderer && "[에러] draw호출시 렌더러가 null입니다.");

	Math::Mat4 view, proj;
	DirectX::XMStoreFloat4x4(&view, viewMat);
	DirectX::XMStoreFloat4x4(&proj, projMat);

	for (const auto& tool : tools_) 
	{
		if (tool->isVisible()) { tool->draw(context_, view, proj); }
	}
}

std::shared_ptr<Render::Tools::Grid> ToolObjectManager::createGrid(int size, float spacing)
{
	auto grid = std::make_shared<Render::Tools::Grid>();
	grid->initialize(device_, size, spacing);
	tools_.push_back(grid);
	return grid;
}
