#include "RenderCoordinator.hpp"
#include "common/Mode.hpp"
#include "common/Math.hpp"
#include "CameraCoordinator.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/uiEvent/UIEventSubscriber.hpp"
#include "event/uiEvent/viewport/GridChangedEvent.hpp"
#include "renderer/dx11/DX11Renderer.hpp"

#include "core/manager/scene/BaseObjectManager.hpp"
#include "core/manager/editor/ToolObjectManager.hpp"
#include "viewport/ui/Grid.hpp"
using namespace std;
using namespace Render;


bool RenderCoordinator::initialize(DX11Renderer* renderer, CameraCoordinator* cameraCoordinator) 
{
	renderer_ = renderer;
	cameraCoordinator_ = cameraCoordinator;

	lightManager_ = std::make_shared<LightManager>();
	if (!lightManager_->initialize()) { return false; }

	BaseObjectManager::get()->initialize(renderer);
	ToolObjectManager::get()->initialize(renderer);


	AppEventSubscriber::get().subscribe<AppModeChangedEvent>([this](const AppModeChangedEvent& event)
		{
			this->onAppModeChanged(event);
		});

	UIEventSubscriber::get().subscribe<WireframeChangedEvent>([this](const WireframeChangedEvent& event)
		{
			this->setShowWireframe(event.isVisible);
		});
	UIEventSubscriber::get().subscribe<SolidChangedEvent>([this](const SolidChangedEvent& event)
		{
			this->setShowSolid(event.isVisible);
		});

	//Grid초기화
	int defaultSize = 10;
	float defaultSpacing = 1.0f;

	grid_ = ToolObjectManager::get()->createGrid(defaultSize, defaultSpacing);
	if (grid_) 
	{
		UIEventSubscriber::get().subscribe<GridChangedEvent>([this](const GridChangedEvent& event)
			{
				this->grid_->setVisible(event.isVisible);
			});
	}
	else 
	{
		assert(false && "[에러] 그리드 초기화 실패");
		return false;
	}
	
	BaseObjectManager::get()->createCube(1.0f);
	return true;
}

void RenderCoordinator::render() 
{
	if (!renderer_ || !cameraCoordinator_) 
	{
		assert(false && "[에러] 렌더링 준비 실패");
		return;
	}

	const Camera& activeCam = cameraCoordinator_->getViewportCameraManager()->getActiveCam();
	updateGlobalRenderData(activeCam);

	DirectX::XMMATRIX viewMat = activeCam.getViewMatrixXM();
	DirectX::XMMATRIX projMat = activeCam.getProjectionMatrixXM();

	if (showSolid_) 
	{
		renderer_->setWireframeMode(false);
		BaseObjectManager::get()->draw(renderer_, viewMat, projMat, nullptr);
	}
	if (showWireframe_) 
	{
		renderer_->setWireframeMode(true);
		BaseObjectManager::get()->draw(renderer_, viewMat, projMat, Render::Material::getWireframePS());
	}
	if (currentMode_ == AppMode::Edit && showToolObjects_) 
	{
		renderer_->setWireframeMode(false);
		ToolObjectManager::get()->draw(renderer_, viewMat, projMat);
	}
}

void RenderCoordinator::onAppModeChanged(const AppModeChangedEvent& event) 
{
	currentMode_ = event.newMode_;

	if (currentMode_ == AppMode::Edit) 
	{
		showSolid_ = true;
		showWireframe_ = true;
		showToolObjects_ = true;
	}
	else 
	{
		showSolid_ = true;
		showWireframe_ = false;
		showToolObjects_ = false;
	}
}

void RenderCoordinator::updateGlobalRenderData(const Camera& activeCam)
{
	auto sunData = lightManager_->getDirectionalLightData();
	auto [ambientTop, ambientMid, ambientBot] = lightManager_->getAmbientColors();

	Render::SceneConstantBufferData sb;
	sb.view = activeCam.getViewMatrix();
	sb.projection = activeCam.getProjectionMatrix();
	sb.cameraPos = activeCam.getPosition();

	sb.sunDirection = sunData.direction;
	sb.sunColor = sunData.color;
	sb.sunIntensity = sunData.intensity;

	sb.ambientTop = ambientTop;
	sb.ambientMid = ambientMid;
	sb.ambientBot = ambientBot;

	renderer_->updateSceneConstants(sb);
}
