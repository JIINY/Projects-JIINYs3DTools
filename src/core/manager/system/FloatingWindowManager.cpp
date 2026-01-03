#include "FloatingWindowManager.hpp"
#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/ui/EnvironmentConfigRequestedEvent.hpp"
#include "event/appEvent/ui/EnvironmentConfigChangedEvent.hpp"
#include "ui/window/environmentConfig/EnvironmentConfig.hpp"
#include "core/manager/scene/LightManager.hpp"

#include "event/uiEvent/UIEventPublisher.hpp"
#include "event/uiEvent/UIEventSubscriber.hpp"
#include "event/uiEvent/viewport/CameraInfoRequestedEvent.hpp"
#include "event/uiEvent/viewport/CameraInfoChangedEvent.hpp"
#include "core/manager/editor/ViewportCameraManager.hpp"


bool FloatingWindowManager::initialize(FloatingWindowContext context, FloatingConfigData data)
{
	if (!context.lightManager || !context.cameraManager) 
	{ 
		assert(false && "[에러] FloatingWindowManager 초기값이 비었습니다.\n");
		return false; 
	}

	lightManager_ = context.lightManager;
	camManager_ = context.cameraManager;

	isEnvConfigVisible_ = data.showEnvConfig;
	isCamInfoVisible_ = data.showCameraInfo;

	envConfig_.initialize(lightManager_);
	cameraInfoUI_.initialize(camManager_);


	//콜백 구독 예약
	auto envConfigID = AppEventSubscriber::get().subscribe<EnvironmentConfigRequestedEvent>([this](const EnvironmentConfigRequestedEvent& event) 
		{
			this->setEnvironmentConfigVisibility(event.isVisible);
		});
	AppEventSubID_.push_back(envConfigID);

	auto camInfoSubID = UIEventSubscriber::get().subscribe<CameraInfoRequestedEvent>([this](const CameraInfoRequestedEvent& event)
		{
			this->setCameraInfoVisibility(event.isVisible);
		});
	UIEventSubID_.push_back(camInfoSubID);

	return true;
}

void FloatingWindowManager::draw()
{
	if (isEnvConfigVisible_) 
	{
		envConfig_.draw(isEnvConfigVisible_, envSerializer_);
	}

	if (isCamInfoVisible_)
	{
		cameraInfoUI_.draw(isCamInfoVisible_);
	}
}

void FloatingWindowManager::setEnvironmentConfigVisibility(bool isVisible) 
{
	if (isEnvConfigVisible_ == isVisible) { return; }

	isEnvConfigVisible_ = isVisible;
	AppEventPublisher::get().publish(EnvironmentConfigChangedEvent{ isEnvConfigVisible_ });
}

void FloatingWindowManager::setCameraInfoVisibility(bool isVisible)
{
	if (isCamInfoVisible_ == isVisible) { return; }

	isCamInfoVisible_ = isVisible;
	UIEventPublisher::get().publish(CameraInfoChangedEvent{ isCamInfoVisible_ });
}

void FloatingWindowManager::toggleCameraInfo()
{
	setCameraInfoVisibility(!isCamInfoVisible_);
}
