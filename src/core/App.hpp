#pragma once
#include <memory>

class InputHandler;
class MainMenuBarUI;

class AppUIManager;
class ShortcutManager;
class FloatingWindowManager;

class CameraCoordinator;
class RenderCoordinator;
class ViewStateCoordinator;
class ResourceCoordinator;

class AppEventQueue;
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

	MainMenuBarUI* getMainMenuBar() { return mainMenuBarUI_.get(); }

	AppUIManager* getAppUIManager() { return appUIManager_.get(); }
	const AppUIManager* getAppUIManager() const { return appUIManager_.get(); }

	DX11Renderer* getDX11Renderer() { return renderer_.get(); }


protected:
	void handleInput();


private:
	std::unique_ptr<InputHandler> inputHandler_;
	
	std::unique_ptr<AppUIManager> appUIManager_;
	std::unique_ptr<ShortcutManager> shortcutManager_;
	std::unique_ptr<FloatingWindowManager> floatingWindowManager_;

	std::unique_ptr<CameraCoordinator> cameraCoordinator_;
	std::unique_ptr<RenderCoordinator> renderCoordinator_;
	std::unique_ptr<ViewStateCoordinator> viewStateCoordinator_;
	std::unique_ptr<ResourceCoordinator> resourceCoordinator_;

	std::unique_ptr<MainMenuBarUI> mainMenuBarUI_;

	std::unique_ptr<DX11Renderer> renderer_;
};
