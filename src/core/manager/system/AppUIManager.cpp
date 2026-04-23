#include "AppUIManager.hpp"
#include <vector>
#include <memory>
#include <variant>
#include <type_traits>
#include "common/Mode.hpp"
#include "common/ElementID.hpp"
#include "core/App.hpp"
#include "ui/appUI/AppUI.hpp"

#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/state/AppModeChangedEvent.hpp"
#include "event/appEvent/state/AppModeCycleRequestedEvent.hpp"
#include "event/appEvent/state/AppModeSetRequestedEvent.hpp"

#include "viewport/camera/CameraInclude.hpp"
using namespace std;


AppUIManager::AppUIManager() : appUI_(make_unique<AppUI>()) {}
AppUIManager::~AppUIManager() = default;

void AppUIManager::initialize() 
{
	appUI_->initialize(this);

	auto appModeCycleID = AppEventSubscriber::get().subscribe<AppModeCycleRequestedEvent>([this](const AppModeCycleRequestedEvent& event)
		{
			this->onAppModeCycleRequested(event);
		});
	appEventSubID_.push_back(appModeCycleID);
	auto appModeSetID = AppEventSubscriber::get().subscribe<AppModeSetRequestedEvent>([this](const AppModeSetRequestedEvent& event)
		{
			this->onAppModeSetRequested(event);
		});
	appEventSubID_.push_back(appModeSetID);
}

void AppUIManager::onAppModeCycleRequested(const AppModeCycleRequestedEvent& event) 
{
	AppMode oldMode = currentMode_;
	currentMode_ = getNextMode(currentMode_);
	AppEventPublisher::get().publish(AppModeChangedEvent(currentMode_, oldMode));
}

void AppUIManager::onAppModeSetRequested(const AppModeSetRequestedEvent& event) 
{
	if (currentMode_ != event.newMode) 
	{
		AppMode oldMode = currentMode_;
		currentMode_ = event.newMode;
		AppEventPublisher::get().publish(AppModeChangedEvent(currentMode_, oldMode));
	}
}

void AppUIManager::setMode(AppMode newMode)
{
	//프로그램 내부에서 AppMode변경이 필요할 때
	AppEventPublisher::get().publish(AppModeSetRequestedEvent(newMode));
}

AppMode AppUIManager::getNextMode(AppMode mode)
{
	constexpr int count = static_cast<int>(AppMode::Count);
	int next = static_cast<int>(mode) + 1;

	if (next >= count)
		next = 0;

	return static_cast<AppMode>(next);
}

void AppUIManager::draw() 
{
	appUI_->draw();
}
