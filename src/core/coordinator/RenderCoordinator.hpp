#pragma once
#include <memory>
#include "common/Mode.hpp"
#include "core/manager/scene/LightManager.hpp"
#include "viewport/ui/Grid.hpp"

struct AppModeChangedEvent;
class DX11Renderer;
class CameraCoordinator;
class Camera;


struct SceneGlobalData
{
	Render::LightData light;
	Math::Vec3 eyePos;
	float padding;
};


class RenderCoordinator 
{
public:
	bool initialize(DX11Renderer* renderer, CameraCoordinator* cameraCoordinator);
	void render();
	void onAppModeChanged(const AppModeChangedEvent& event);

	void setShowSolid(bool show) { showSolid_ = show; }
	void setShowWireframe(bool show) { showWireframe_ = show; }
	void setAppMode(AppMode mode) { currentMode_ = mode; }
	void setShowTools(bool show) { showToolObjects_ = show; }

	bool getShowSolid() const { return showSolid_; }
	bool getShowWireframe() const { return showWireframe_; }
	LightManager* getLightManager() const { return lightManager_.get(); }
	void updateGlobalRenderData(const Camera& activeCam);

private:
	std::shared_ptr<Render::Tools::Grid> grid_;
	std::shared_ptr<LightManager> lightManager_;
	AppMode currentMode_ = AppMode::Edit;
	bool showSolid_ = true;
	bool showWireframe_ = true;
	bool showToolObjects_ = true;

	DX11Renderer* renderer_ = nullptr;
	CameraCoordinator* cameraCoordinator_ = nullptr;
};
