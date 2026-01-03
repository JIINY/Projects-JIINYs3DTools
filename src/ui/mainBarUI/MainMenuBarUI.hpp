#pragma once
#include "common/Mode.hpp"

struct AppModeChangedEvent;
struct CameraModeChangedEvent;
struct CameraInfoChangedEvent;
struct GridChangedEvent;
struct WireframeChangedEvent;
struct SolidChangedEvent;

struct EnvironmentConfigChangedEvent;


class MainMenuBarUI {
public:
	void initialize();
	void draw();
	void updateCamUIState(CameraMode cam, ViewMode view);

protected:
	void drawMenuBar();
	void fileMenu();
	void editMenu();
	void modeMenu();
	void cameraMenu();
	void windowMenu();
	void prefMenu();


private:
	AppMode currentMode_ = AppMode::Edit;
	CameraUIMode combinedCamUI_ = CameraUIMode::FreeArm; //CameraManager의 setCameraMode()를 통해서만 관리
	bool isCamInfoVisible_ = false;
	bool isGridVisible_ = true;
	bool isWireframeVisible_ = true;
	bool isSolidVisible_ = true;
	
	bool isEnvironmentConfigVisible_ = false;

	void onAppModeChanged(const AppModeChangedEvent& event);
	void onCameraModeChanged(const CameraModeChangedEvent& event);
	void onCameraInfoChanged(const CameraInfoChangedEvent& event);
	void onGridChanged(const GridChangedEvent& event);
	void onWireframeChanged(const WireframeChangedEvent& event);
	void onSolidChanged(const SolidChangedEvent& event);

	void onEnvironmentConfigChanged(const EnvironmentConfigChangedEvent& event);
};
