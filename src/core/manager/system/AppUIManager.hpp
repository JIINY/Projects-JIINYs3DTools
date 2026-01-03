#pragma once
#include "common/Mode.hpp"
#include "ui/appUI/AppUI.hpp"
#include "event/appEvent/state/AppModeCycleRequestedEvent.hpp"
#include "event/appEvent/state/AppModeSetRequestedEvent.hpp"

class App;

class AppUIManager {
public:
	void initialize();
	void draw();

	void setMode(AppMode mode);
	AppMode getMode() const { return currentMode_; }
	AppUI* getAppUI() { return &appUI_; }


private:
	AppUI appUI_;
	AppMode currentMode_ = AppMode::Edit;

	AppMode getNextMode(AppMode mode);

	void onAppModeCycleRequested(const AppModeCycleRequestedEvent& event);
	void onAppModeSetRequested(const AppModeSetRequestedEvent& event);
};
