#pragma once
#include <vector>
#include <memory>
#include "common/Mode.hpp"
#include "event/appEvent/AppEventSubscriber.hpp"

struct AppModeCycleRequestedEvent;
struct AppModeSetRequestedEvent;
class App;
class AppUI;


class AppUIManager {
public:
	AppUIManager();
	~AppUIManager();

	void initialize();
	void draw();

	void setMode(AppMode mode);
	AppMode getMode() const { return currentMode_; }
	AppUI* getAppUI() { return appUI_.get(); }


private:
	std::vector<AppEventSubscriptionID> appEventSubID_;
	std::unique_ptr<AppUI> appUI_;
	AppMode currentMode_ = AppMode::Edit;

	AppMode getNextMode(AppMode mode);

	void onAppModeCycleRequested(const AppModeCycleRequestedEvent& event);
	void onAppModeSetRequested(const AppModeSetRequestedEvent& event);
};
