#include "RenderCoordinator.hpp"
#include <DirectXMath.h>
#include "common/Mode.hpp"
#include "common/Math.hpp"

#include "CameraCoordinator.hpp"
#include "core/manager/editor/ViewportCameraManager.hpp"
#include "../scene/PassiveObjectCoordinator.hpp"
#include "../resources/ResourceCoordinator.hpp"
#include "core/manager/editor/ToolObjectManager.hpp"
#include "core/manager/scene/LightManager.hpp"
#include "render/Material.hpp"

#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/uiEvent/UIEventSubscriber.hpp"
#include "event/uiEvent/viewport/GridChangedEvent.hpp"
#include "render/RenderCommandQueue.hpp"
#include "renderer/dx11/DX11Renderer.hpp"

#include "common/DebugLog.hpp"
using namespace std;
using namespace Render;
using namespace DirectX;

RenderCoordinator::RenderCoordinator() 
	: lightManager_(make_unique<LightManager>()), toolObjManager_(make_unique<ToolObjectManager>()), renderQueue_(make_unique<RenderCommandQueue>()) {}
RenderCoordinator::~RenderCoordinator() = default;


bool RenderCoordinator::initialize(RenderContext context)
{
	assert(context.renderer && "초기화 실패");
	renderer_ = context.renderer;
	assert(context.camCoordinator && "초기화 실패");
	cameraCoordinator_ = context.camCoordinator;
	assert(context.passiveObjCoordinator && "초기화 실패");
	passiveObjCoordinator_ = context.passiveObjCoordinator;
	assert(context.resourceCoordinator && "초기화 실패");
	resourceCoordinator_ = context.resourceCoordinator;

	if (!lightManager_->initialize()) { return false; }

	ToolObjectContext toolContext;
	toolContext.renderer = renderer_;
	toolContext.viewCamManager = cameraCoordinator_->getViewportCameraManager();
	toolContext.matManager = resourceCoordinator_->getMaterialManager();
	if (!toolObjManager_->initialize(toolContext)) { return false; };

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

	return true;
}

void RenderCoordinator::render() 
{
	assert(renderer_ && "renderer없음, 렌더링 준비 실패");
	assert(cameraCoordinator_ && "camCoordinator없음, 렌더링 준비 실패");
	assert(passiveObjCoordinator_ && "passiveObjCoordinator없음, 렌더링 준비 실패");
	assert(renderQueue_ && "renderQueue없음, 렌더링 준비 실패");

	const Camera& activeCam = cameraCoordinator_->getViewportCameraManager()->getActiveCam();
	updateGlobalRenderData(activeCam);

	XMMATRIX viewMat = activeCam.getViewMatrixXM();
	XMMATRIX projMat = activeCam.getProjectionMatrixXM();
	Math::Mat4 view, proj;
	XMStoreFloat4x4(&view, viewMat);
	XMStoreFloat4x4(&proj, projMat);

	if (currentMode_ == AppMode::Edit && showToolObjects_)
	{
		//Grid
		renderQueue_->clear();
		toolObjManager_->addGridToRenderQueue(renderQueue_.get(), viewMat);
		renderQueue_->sort();

		renderer_->setWireframeMode(false);
		renderQueue_->execute(renderer_->getDeviceContext(), view, proj);
	}

	if (showSolid_)
	{
		renderQueue_->clear();
		passiveObjCoordinator_->addToRenderQueue(renderQueue_.get(), viewMat, OverridePSType::None);

		renderQueue_->sort();
		renderer_->setWireframeMode(false);
		renderQueue_->execute(renderer_->getDeviceContext(), view, proj);
	}
	if (showWireframe_) 
	{
		renderQueue_->clear();
		passiveObjCoordinator_->addToRenderQueue(renderQueue_.get(), viewMat, OverridePSType::Black);

		renderQueue_->sort();
		renderer_->setWireframeMode(true);
		renderQueue_->execute(renderer_->getDeviceContext(), view, proj);
	}

	if (currentMode_ == AppMode::Edit && showToolObjects_)
	{
		//Gizmo
		renderQueue_->clear();
		toolObjManager_->addGizmoToRenderQueue(renderQueue_.get(), viewMat);
		renderQueue_->sort();

		renderer_->getDeviceContext()->ClearDepthStencilView(renderer_->getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		renderer_->setWireframeMode(false);
		renderQueue_->execute(renderer_->getDeviceContext(), view, proj);
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

void RenderCoordinator::drawImGui() 
{
	toolObjManager_->drawImGui();
}

void RenderCoordinator::update(float deltaTime, const Math::Ray& mouseRay)
{
	toolObjManager_->update(deltaTime, mouseRay);
}

void RenderCoordinator::updateTransforms()
{
	passiveObjCoordinator_->updateTransforms();
}
