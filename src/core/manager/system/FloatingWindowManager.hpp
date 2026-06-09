#pragma once
#include <memory>
#include <vector>
#include "core/AppConfig.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/uiEvent/UIEventSubscriber.hpp"

struct CreatePopupChangedEvent;
struct EnvironmentConfigPopupChangedEvent;
struct MaterialPopupChangedEvent;
struct CameraInfoChangedEvent;

class MaterialManager;
class LightManager;
class ViewportCameraManager;
class PassiveObjectCoordinator;
class CameraInfoUI;
namespace Create { class CreatePanel; }
namespace EnvConfig { class EnvironmentConfig; }
namespace MaterialEditor { class MaterialPanel; }


struct FloatingWindowContext 
{
	LightManager* lightManager = nullptr;
	MaterialManager* materialManager = nullptr;
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
		return { isCreateVisible_, isEnvConfigVisible_, isMaterialVisible_, isCamInfoVisible_ };
	}

	void setCreateVisibility(bool isVisible);
	void setEnvironmentConfigVisibility(bool isVisible);
	void setMaterialVisibility(bool isVisible);
	void setCameraInfoVisibility(bool isVisible);

	EnvConfig::EnvironmentConfig* getEnvironmentConfig() const { return envConfig_.get(); }

	void toggleCameraInfo();
	bool isCamInfoVisible() const { return isCamInfoVisible_; }


private:
	std::vector<AppEventSubscriptionID> appEventSubID_;
	std::unique_ptr<Create::CreatePanel> create_;
	bool isCreateVisible_ = false;
	std::unique_ptr<EnvConfig::EnvironmentConfig> envConfig_;
	bool isEnvConfigVisible_ = false;
	std::unique_ptr<MaterialEditor::MaterialPanel> material_;
	bool isMaterialVisible_ = false;

	std::vector<UIEventSubscriptionID> uiEventSubID_;
	std::unique_ptr<CameraInfoUI> cameraInfoUI_;
	bool isCamInfoVisible_ = false;
	ViewportCameraManager* camManager_ = nullptr;

	void onCreatePopupChanged(const CreatePopupChangedEvent& event);
	void onEnvironmentConfigPopupChanged(const EnvironmentConfigPopupChangedEvent& event);
	void onMaterialPopupChanged(const MaterialPopupChangedEvent& event);
	void onCameraInfoChanged(const CameraInfoChangedEvent& event);
};
