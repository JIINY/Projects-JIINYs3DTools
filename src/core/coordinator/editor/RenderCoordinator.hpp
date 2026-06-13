#pragma once
#include <memory>
#include "common/Mode.hpp"
#include "common/Math.hpp"
#include "render/lights/LightType.hpp"
#include "render/RenderCommandQueue.hpp"

struct AppModeChangedEvent;
class DX11Renderer;
class CameraCoordinator;
class Camera;
class PassiveObjectCoordinator;
class ResourceCoordinator;
class LightManager;
class ToolObjectManager;


struct SceneGlobalData
{
	Render::LightData light = {};
	Math::Vec3 eyePos = { 0.0f, 0.0f, 0.0f };
	float padding = 0.0f;
};

struct RenderContext 
{
	DX11Renderer* renderer = nullptr;
	CameraCoordinator* camCoordinator = nullptr;
	PassiveObjectCoordinator* passiveObjCoordinator = nullptr;
	ResourceCoordinator* resourceCoordinator = nullptr;
};

class RenderCoordinator 
{
public:
	RenderCoordinator();
	~RenderCoordinator();

	bool initialize(RenderContext context);
	void drawImGui();
	void render();
	void update(float deltaTime, const Math::Ray& mouseRay);

	void onAppModeChanged(const AppModeChangedEvent& event);
	void updateTransforms();

	void setShowSolid(bool show) { showSolid_ = show; }
	void setShowWireframe(bool show) { showWireframe_ = show; }
	void setAppMode(AppMode mode) { currentMode_ = mode; }
	void setShowTools(bool show) { showToolObjects_ = show; }

	bool getShowSolid() const { return showSolid_; }
	bool getShowWireframe() const { return showWireframe_; }
	LightManager* getLightManager() const { return lightManager_.get(); }
	ToolObjectManager* getToolObjectManager() const { return toolObjManager_.get(); }
	void updateGlobalRenderData(const Camera& activeCam);


private:
	std::unique_ptr<LightManager> lightManager_;
	std::unique_ptr<ToolObjectManager> toolObjManager_;
	std::unique_ptr<Render::RenderCommandQueue> renderQueue_;

	AppMode currentMode_ = AppMode::Edit;
	bool showSolid_ = true;
	bool showWireframe_ = true;
	bool showToolObjects_ = true;
	bool showCollider_ = true;

	DX11Renderer* renderer_ = nullptr;
	CameraCoordinator* cameraCoordinator_ = nullptr;
	PassiveObjectCoordinator* passiveObjCoordinator_ = nullptr;
	ResourceCoordinator* resourceCoordinator_ = nullptr;
};
