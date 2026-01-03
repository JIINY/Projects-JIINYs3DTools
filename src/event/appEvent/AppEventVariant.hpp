#pragma once
#include <variant>
#include "state/AppModeChangedEvent.hpp"
#include "state/AppModeCycleRequestedEvent.hpp"
#include "state/AppModeSetRequestedEvent.hpp"
#include "state/CameraModeChangedEvent.hpp"
#include "state/WindowSizeChangedEvent.hpp"
#include "ui/EnvironmentConfigChangedEvent.hpp"
#include "ui/EnvironmentConfigRequestedEvent.hpp"

using AppEventVariant = std::variant<
	//Flow
//	SceneLoadStartEvent,
//	AppShutdownEvent,

	//State
	AppModeChangedEvent,
	AppModeCycleRequestedEvent,
	AppModeSetRequestedEvent,
	CameraModeChangedEvent,
	WindowSizeChangedEvent,

	//ui
	EnvironmentConfigChangedEvent,
	EnvironmentConfigRequestedEvent
>;
