#pragma once
#include <memory>
#include <vector>
#include "core/AppConfig.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/ui/EnvironmentConfigChangedEvent.hpp"
#include "event/appEvent/ui/EnvironmentDataRequestedEvent.hpp"
#include "ui/window/environmentConfig/EnvironmentConfig.hpp"

#include "event/uiEvent/UIEventSubscriber.hpp"
#include "event/uiEvent/viewport/CameraInfoChangedEvent.hpp"
#include "ui/preference/CameraInfoUI.hpp"

class ViewportCameraManager;
class LightManager;
namespace EnvironmentConfig
{
	class EnvironmentConfigSerializer;
}

struct FloatingWindowContext 
{
	ViewportCameraManager* cameraManager = nullptr;
	LightManager* lightManager = nullptr;
	EnvConfig::EnvironmentConfigSerializer* envSerializer = nullptr;
};

class FloatingWindowManager
{
public:
	bool initialize(FloatingWindowContext context, FloatingConfigData data);
	void draw();

	FloatingConfigData getCurrentState() const 
	{
		return { isEnvConfigVisible_, isCamInfoVisible_ };
	}

	void setEnvironmentConfigVisibility(bool isVisible);
	void setEnvironmentConfigData(const EnvironmentDataRequestedEvent& event);

	void toggleCameraInfo();
	void setCameraInfoVisibility(bool isVisible);
	bool isCamInfoVisible() const { return isCamInfoVisible_; }



private:
	std::vector<AppEventSubscriptionID> AppEventSubID_;
	EnvConfig::EnvironmentConfig envConfig_;
	LightManager* lightManager_ = nullptr;

	std::vector<UIEventSubscriptionID> UIEventSubID_;
	CameraInfoUI cameraInfoUI_;
	ViewportCameraManager* camManager_ = nullptr;

	bool isEnvConfigVisible_ = false;
	bool isCamInfoVisible_ = false;

	EnvConfig::EnvironmentConfigSerializer* envSerializer_ = nullptr;

	void onEnvironmentConfigChanged(const EnvironmentConfigChangedEvent& event);
	void onCameraInfoChanged(const CameraInfoChangedEvent& event);
};
