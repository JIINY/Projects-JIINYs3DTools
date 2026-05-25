#pragma once
#include <memory>
#include <vector>
#include "core/AppConfig.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/uiEvent/UIEventSubscriber.hpp"

struct CreatePopupChangedEvent;
struct EnvironmentConfigPopupChangedEvent;
struct CameraInfoChangedEvent;

class LightManager;
class ViewportCameraManager;
class PassiveObjectCoordinator;
class CameraInfoUI;
namespace Create { class CreatePanel; }
namespace EnvConfig { class EnvironmentConfig; }

struct FloatingWindowContext 
{
	LightManager* lightManager = nullptr;
	ViewportCameraManager* cameraManager = nullptr;
	PassiveObjectCoordinator* passiveObjCoordinator = nullptr;
};

class FloatingWindowManager
{
public:
	FloatingWindowManager();
	~FloatingWindowManager();

	bool initialize(const FloatingWindowContext& context, const FloatingConfigData& data);
	void draw();

	FloatingConfigData getCurrentState() const 
	{
		return { isCreateVisible_, isEnvConfigVisible_, isCamInfoVisible_ };
	}

	void setCreateVisibility(bool isVisible);
	void setEnvironmentConfigVisibility(bool isVisible);
	void setCameraInfoVisibility(bool isVisible);

	void toggleCameraInfo();
	bool isCamInfoVisible() const { return isCamInfoVisible_; }


private:
	std::vector<AppEventSubscriptionID> appEventSubID_;
	std::unique_ptr<Create::CreatePanel> create_;
	bool isCreateVisible_ = false;
	std::unique_ptr<EnvConfig::EnvironmentConfig> envConfig_;
	bool isEnvConfigVisible_ = false;

	std::vector<UIEventSubscriptionID> uiEventSubID_;
	std::unique_ptr<CameraInfoUI> cameraInfoUI_;
	bool isCamInfoVisible_ = false;
	ViewportCameraManager* camManager_ = nullptr;

	void onCreatePopupChanged(const CreatePopupChangedEvent& event);
	void onEnvironmentConfigPopupChanged(const EnvironmentConfigPopupChangedEvent& event);
	void onCameraInfoChanged(const CameraInfoChangedEvent& event);
};
