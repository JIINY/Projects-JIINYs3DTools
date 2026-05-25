#include "EnvironmentConfigController.hpp"
#include <string>
#include <filesystem>
#include "core/PathConfig.hpp"
#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/ui/EnvironmentDataRequestedEvent.hpp"
#include "event/appEvent/ui/EnvironmentDataChangedEvent.hpp"
#include "ui/window/environmentConfig/EnvironmentConfigSerializer.hpp"
#include "core/manager/scene/LightManager.hpp"

#include "common/DebugLog.hpp"
using namespace std;


namespace EnvConfig 
{
	bool EnvironmentConfigController::initialize(const EnvironmentContext& context)
	{
		assert(context.lightManager && "LightManager가 비었습니다. 초기화 실패");
		if (!context.lightManager) { return false; }
		lightManager_ = context.lightManager;

		EnvConfig::EnvConfigContext envContext;
		envContext.lightManager = lightManager_;

		envSerializer_.initialize(envContext);
		envSerializer_.deserialize(EnvPathConfig::defaultFilePath_);
		backupGlobalLight_ = lightManager_->backupCurrentGlobalLightData();


		//콜백 구독 예약
		auto envDataID = AppEventSubscriber::get().subscribe<EnvironmentDataRequestedEvent>([this](const EnvironmentDataRequestedEvent& event)
			{
				this->onEnvironmentRequested(event);
			});
		appEventSubID_.push_back(envDataID);

		return true;
	}

	void EnvironmentConfigController::onEnvironmentRequested(const EnvironmentDataRequestedEvent& event)
	{
		switch (event.type)
		{
		case EnvActionType::Save:
		case EnvActionType::SaveAs:
		{
			bool result = envSerializer_.serialize(event.path);
			if (result)
			{
				backupGlobalLight_ = lightManager_->backupCurrentGlobalLightData();
				AppEventPublisher::get().publish(EnvironmentDataChangedEvent{ EnvDataType::FileSaved, event.path });
			}
			break;
		}
		case EnvActionType::Load:
		{
			bool result = envSerializer_.deserialize(event.path);
			if (result) 
			{
				backupGlobalLight_ = lightManager_->backupCurrentGlobalLightData();
				AppEventPublisher::get().publish(EnvironmentDataChangedEvent{ EnvDataType::FileLoaded, event.path });
			}
			break;
		}
		case EnvActionType::Restore:
		{
			lightManager_->restoreCurrentGlobalLightData(backupGlobalLight_);
			AppEventPublisher::get().publish(EnvironmentDataChangedEvent{ EnvDataType::FileLoaded, event.path });
			break;
		}
		case EnvActionType::New:
		{
			bool result = envSerializer_.deserialize(EnvPathConfig::defaultFilePath_);
			if (result)
			{
				backupGlobalLight_ = lightManager_->backupCurrentGlobalLightData();
				AppEventPublisher::get().publish(EnvironmentDataChangedEvent{ EnvDataType::FileLoaded, "" });
			}
			break;
		}
		case EnvActionType::UIModify: 
		{
			AppEventPublisher::get().publish(EnvironmentDataChangedEvent{ EnvDataType::DataChanged, "" });
			break;
		}
		default:
			assert(0 && "초기 EnvActionType이 이벤트로 들어왔습니다.");
			break;
		}
	}
}
