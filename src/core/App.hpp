#pragma once
#include <memory>
#include <Windows.h>
#include <string>
#include <vector>
#include "event/appEvent/AppEventSubscriber.hpp"

struct SceneDataChangedEvent;
class EnvironmentConfigController;
class MainMenuBarUI;
class MessagePopupController;

class InputHandler;
class InputEventEditorHandler;
class SceneObjectActionHandler;

class AppUIManager;
class ShortcutManager;
class FloatingWindowManager;
class SceneFileManager;
class SceneObjectManager;

class CameraCoordinator;
class RenderCoordinator;
class ViewStateCoordinator;
class SelectionCoordinator;
class ResourceCoordinator;

namespace Scene { class SceneSerializer; }
namespace EnvConfig { class EnvironmentConfigController; }
class AppEventSubscriber;
class DX11Renderer;


class App {
public:
	App();
	~App();

	bool initialize(void* hwnd);
	void run();
	void update(float deltaTime);
	void draw();
	void render();
	void shutdown();

	void onScreenResize(int width, int height);
	int getScreenWidth() const { return width_; }
	int getScreenHeight() const { return height_; }
	float getAspectRatio() const { return static_cast<float>(width_) / static_cast<float>(height_); }
	MainMenuBarUI* getMainMenuBar() { return mainMenuBarUI_.get(); }

	AppUIManager* getAppUIManager() { return appUIManager_.get(); }
	const AppUIManager* getAppUIManager() const { return appUIManager_.get(); }

	DX11Renderer* getDX11Renderer() { return renderer_.get(); }


private:
	int width_ = 1280;
	int height_ = 800;

	HWND hwnd_ = nullptr;
	std::string currentSceneTitle_ = "";
	std::vector<AppEventSubscriptionID> appEventSubID_;

	std::unique_ptr<MessagePopupController> msgPopupController_;
	std::unique_ptr<EnvConfig::EnvironmentConfigController> envConfigController_;
	std::unique_ptr<InputHandler> inputHandler_;
	std::unique_ptr<InputEventEditorHandler> inputEditorHandler_;
	std::unique_ptr<SceneObjectActionHandler> sceneObjActionHandler_;
	
	std::unique_ptr<AppUIManager> appUIManager_;
	std::unique_ptr<ShortcutManager> shortcutManager_;
	std::unique_ptr<FloatingWindowManager> floatingWindowManager_;
	std::unique_ptr<SceneFileManager> sceneFileManager_;
	std::unique_ptr<SceneObjectManager> sceneObjManager_;

	std::unique_ptr<CameraCoordinator> cameraCoordinator_;
	std::unique_ptr<RenderCoordinator> renderCoordinator_;
	std::unique_ptr<ViewStateCoordinator> viewStateCoordinator_;
	std::unique_ptr<SelectionCoordinator> selectionCoordinator_;
	std::unique_ptr<ResourceCoordinator> resourceCoordinator_;

	std::unique_ptr<MainMenuBarUI> mainMenuBarUI_;
	std::unique_ptr<DX11Renderer> renderer_;

	void onSceneDataChanged(const SceneDataChangedEvent& event);
};
