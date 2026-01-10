#include "FloatingWindowManager.hpp"
#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/ui/EnvironmentConfigRequestedEvent.hpp"
#include "event/appEvent/ui/EnvironmentConfigChangedEvent.hpp"
#include "event/appEvent/ui/EnvironmentDataRequestedEvent.hpp"
#include "event/appEvent/ui/EnvironmentDataChangedEvent.hpp"
#include "ui/window/environmentConfig/EnvironmentConfig.hpp"
#include "ui/window/environmentConfig/EnvironmentConfigSerializer.hpp"
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
		assert(0 && "[에러] FloatingWindowManager 초기값이 비었습니다.\n");
		return false; 
	}

	lightManager_ = context.lightManager;
	camManager_ = context.cameraManager;
	envSerializer_ = context.envSerializer;

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

	auto envDataID = AppEventSubscriber::get().subscribe<EnvironmentDataRequestedEvent>([this](const EnvironmentDataRequestedEvent& event)
		{
			this->setEnvironmentConfigData(event);
		});
	AppEventSubID_.push_back(envDataID);

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

void FloatingWindowManager::setEnvironmentConfigData(const EnvironmentDataRequestedEvent& event) 
{
	if (!envSerializer_) 
	{
		assert(0 && "[에러] EnvrionmentSerializer가 없습니다.");
		return;
	}

	switch (event.type) 
	{
	case EnvDataType::Save: 
	{
		break;
	}
	case EnvDataType::SaveAs: 
	{
		break;
	}
	case EnvDataType::Load:
	{
		break;
	}
	case EnvDataType::Restore:
	{
		break;
	}
	case EnvDataType::New:
	{
		bool result = envSerializer_->deserialize(event.filepath);
		if (result) 
		{ 
			AppEventPublisher::get().publish(EnvironmentDataChangedEvent{}); 
		}
		break;
	}
	default:
		assert(0 && "[에러] 초기 EnvDataType이 이벤트로 들어왔습니다.");
		break;
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
