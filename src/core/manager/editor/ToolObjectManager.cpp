#include "ToolObjectManager.hpp"
#include <vector>
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>
#include "common/Math.hpp"
#include "../resources/MaterialManager.hpp"
#include "renderer/Renderer.hpp"
#include "render/RenderCommandQueue.hpp"
#include "render/RenderObject.hpp"
#include "viewport/camera/CameraController.hpp"
#include "viewport/ui/Grid.hpp"
#include "viewport/ui/WorldPivotUI.hpp"
#include "core/manager/editor/ViewportCameraManager.hpp"
#include "viewport/ui/GizmoController.hpp"

#include "event/uiEvent/UIEventSubscriber.hpp"
#include "event/uiEvent/viewport/GridChangedEvent.hpp"
#include "imgui.h"

#include "common/DebugLog.hpp"
using namespace std;
using namespace Render;
using namespace DirectX;


ToolObjectManager::ToolObjectManager() : 
	grid_(make_unique<Render::Tools::Grid>()), worldPivotUI_(std::make_unique<WorldPivotUI>()), gizmoController_(std::make_unique<GizmoController>()) {}
ToolObjectManager::~ToolObjectManager() = default;

bool ToolObjectManager::initialize(ToolObjectContext context)
{
	assert(context.renderer && "초기화 실패");
	device_ = context.renderer->getDevice();
	assert(device_ && "초기화 실패");
	context_ = context.renderer->getDeviceContext();
	assert(context_ && "초기화 실패");

	assert(context.viewCamManager && "초기화 실패");
	viewCamManager_ = context.viewCamManager;
	assert(context.matManager && "초기화 실패");
	materialManager_ = context.matManager;
	assert(gizmoController_ && "초기화 실패");

	if (!gizmoController_->initialize(device_, materialManager_)) { return false; }


	//Grid초기화
	Render::Tools::GridContext gridContext;
	gridContext.device = device_;
	gridContext.matManager = materialManager_;
	gridContext.size = 10;
	gridContext.spacing = 1.0f;
	if (!grid_->initialize(gridContext)) { return false; }

	UIEventSubscriber::get().subscribe<GridChangedEvent>([this](const GridChangedEvent& event)
		{
			this->onGridChanged(event);
		});
	return true;
}

void ToolObjectManager::onGridChanged(const GridChangedEvent& event)
{
	this->showGrid_ = event.isVisible;
	grid_->setGridSize(event.gridSize);
	grid_->setGridSpacing(event.gridSpacing);
}

void ToolObjectManager::drawImGui() 
{
	ImVec2 windowPos_ = ImGui::GetMainViewport()->Pos;
	ImVec2 windowSize_ = ImGui::GetMainViewport()->Size;

	DebugLog("ImGui: pos=[%.1f, %.1f] size=[%.1f, %.1f]", windowPos_.x, windowPos_.y, windowSize_.x, windowSize_.y);

	auto* activeCamController = viewCamManager_->getActiveCamController();
	if (activeCamController != nullptr)
	{
		worldPivotUI_->draw(activeCamController, windowPos_, windowSize_);
	}
}

void ToolObjectManager::addGridToRenderQueue(RenderCommandQueue* queue, const XMMATRIX& viewMat)
{
	if (showGrid_ && grid_)
	{
		grid_->addToRenderQueue(queue, viewMat);
	}
}

void ToolObjectManager::addGizmoToRenderQueue(RenderCommandQueue* queue, const XMMATRIX& viewMat)
{
	if (gizmoController_)
	{
		gizmoController_->addToRenderQueue(queue, viewMat);
	}
}

void ToolObjectManager::addRenderQueue(RenderCommandQueue* queue, const XMMATRIX& viewMat)
{
	if (!queue) { return; }

	XMMATRIX invView = XMMatrixInverse(nullptr, viewMat);
	XMVECTOR camPos = invView.r[3];

	for (const auto& t : tools_)
	{
		if (!t || !t->isVisible()) { continue; }

	}
}

void ToolObjectManager::update(float deltaTime, const Math::Ray& mouseRay)
{
	gizmoController_->update(deltaTime, mouseRay);
}
