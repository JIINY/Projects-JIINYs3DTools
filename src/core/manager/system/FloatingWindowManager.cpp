#include "FloatingWindowManager.hpp"
#include "command/CommandStack.hpp"
#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/ui/CreatePopupChangedEvent.hpp"
#include "event/appEvent/ui/CreatePopupRequestedEvent.hpp"
#include "event/appEvent/ui/EnvironmentConfigPopupRequestedEvent.hpp"
#include "event/appEvent/ui/EnvironmentConfigPopupChangedEvent.hpp"
#include "ui/window/create/CreatePanel.hpp"
#include "ui/window/environmentConfig/EnvironmentConfig.hpp"

#include "event/uiEvent/UIEventPublisher.hpp"
#include "event/uiEvent/UIEventSubscriber.hpp"
#include "event/uiEvent/viewport/CameraInfoRequestedEvent.hpp"
#include "event/uiEvent/viewport/CameraInfoChangedEvent.hpp"
#include "core/manager/editor/ViewportCameraManager.hpp"
#include "core/coordinator/scene/PassiveObjectCoordinator.hpp"
#include "ui/preference/CameraInfoUI.hpp"
using namespace std;


FloatingWindowManager::FloatingWindowManager() :
	create_(make_unique<Create::CreatePanel>()),
	envConfig_(make_unique<EnvConfig::EnvironmentConfig>()),
	cameraInfoUI_(make_unique<CameraInfoUI>()) {}

FloatingWindowManager::~FloatingWindowManager() = default;

bool FloatingWindowManager::initialize(const FloatingWindowContext& context, const FloatingConfigData& data)
{
	assert(context.lightManager && "비었습니다. 초기화 실패");
	assert(context.cameraManager && "비었습니다. 초기화 실패");
	assert(context.passiveObjCoordinator && "비었습니다. 초기화 실패");
	if (!context.lightManager || !context.cameraManager || !context.passiveObjCoordinator)
	{ 
		return false; 
	}

	camManager_ = context.cameraManager;

	isCreateVisible_ = data.showCreate;
	isEnvConfigVisible_ = data.showEnvConfig;
	isCamInfoVisible_ = data.showCameraInfo;

	if (!create_->initialize(context.passiveObjCoordinator)) { return false; }
	if (!envConfig_->initialize(context.lightManager)) { return false; };
	cameraInfoUI_->initialize(camManager_);

	//콜백 구독 예약
	auto createID = AppEventSubscriber::get().subscribe<CreatePopupRequestedEvent>([this](const CreatePopupRequestedEvent& event)
		{
			this->setCreateVisibility(event.isVisible);
		});
	appEventSubID_.push_back(createID);

	auto envConfigID = AppEventSubscriber::get().subscribe<EnvironmentConfigPopupRequestedEvent>([this](const EnvironmentConfigPopupRequestedEvent& event)
		{
			this->setEnvironmentConfigVisibility(event.isVisible);
		});
	appEventSubID_.push_back(envConfigID);

	auto camInfoSubID = UIEventSubscriber::get().subscribe<CameraInfoRequestedEvent>([this](const CameraInfoRequestedEvent& event)
		{
			this->setCameraInfoVisibility(event.isVisible);
		});
	uiEventSubID_.push_back(camInfoSubID);

	return true;
}

void FloatingWindowManager::draw()
{
	if (isCreateVisible_) 
	{
		create_->draw(isCreateVisible_);
	}
	if (isEnvConfigVisible_) 
	{
		envConfig_->draw(isEnvConfigVisible_);
	}
	if (isCamInfoVisible_)
	{
		cameraInfoUI_->draw(isCamInfoVisible_);
	}
}

void FloatingWindowManager::setCreateVisibility(bool isVisible) 
{
	if (isCreateVisible_ == isVisible) { return; }

	isCreateVisible_ = isVisible;
	AppEventPublisher::get().publish(CreatePopupChangedEvent{ isCreateVisible_ });
}

void FloatingWindowManager::setEnvironmentConfigVisibility(bool isVisible) 
{
	if (isEnvConfigVisible_ == isVisible) { return; }

	isEnvConfigVisible_ = isVisible;
	AppEventPublisher::get().publish(EnvironmentConfigPopupChangedEvent{ isEnvConfigVisible_ });
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
