#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>
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

	std::vector<FloatingWindowInfo> getCurrentState() { return floatingInfo_; }
	FloatingWindowInfo* findWindowInfo(const std::string& configKey);

	void setCreateVisibility(bool isVisible);
	void setEnvironmentConfigVisibility(bool isVisible);
	void setMaterialVisibility(bool isVisible);
	void setCameraInfoVisibility(bool isVisible);

	EnvConfig::EnvironmentConfig* getEnvironmentConfig() const { return envConfig_.get(); }

	void toggleCameraInfo();
	bool isCamInfoVisible();


private:
	std::vector<AppEventSubscriptionID> appEventSubID_;
	std::unique_ptr<Create::CreatePanel> create_;
	std::unique_ptr<EnvConfig::EnvironmentConfig> envConfig_;
	std::unique_ptr<MaterialEditor::MaterialPanel> material_;

	std::vector<UIEventSubscriptionID> uiEventSubID_;
	std::unique_ptr<CameraInfoUI> cameraInfoUI_;

	std::vector<FloatingWindowInfo> floatingInfo_;
	std::unordered_map<std::string, std::function<void(bool&)>> drawMap_;
	float prevWidth_ = 0.0f;
	float prevHeight_ = 0.0f;
	ViewportCameraManager* camManager_ = nullptr;

	void onCreatePopupChanged(const CreatePopupChangedEvent& event);
	void onEnvironmentConfigPopupChanged(const EnvironmentConfigPopupChangedEvent& event);
	void onMaterialPopupChanged(const MaterialPopupChangedEvent& event);
	void onCameraInfoChanged(const CameraInfoChangedEvent& event);

	void onWindowSizeChanged(int width, int height);
};
