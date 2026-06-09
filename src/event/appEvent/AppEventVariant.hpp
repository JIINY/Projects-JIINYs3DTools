#pragma once
#include <variant>
#include "flow/SceneDataChangedEvent.hpp"
#include "flow/SceneDataRequestedEvent.hpp"
#include "state/AppModeChangedEvent.hpp"
#include "state/AppModeCycleRequestedEvent.hpp"
#include "state/AppModeSetRequestedEvent.hpp"
#include "state/CameraModeChangedEvent.hpp"
#include "state/SceneObjectChangedEvent.hpp"
#include "state/WindowSizeChangedEvent.hpp"
#include "ui/CreatePopupChangedEvent.hpp"
#include "ui/CreatePopupRequestedEvent.hpp"
#include "ui/EnvironmentConfigPopupChangedEvent.hpp"
#include "ui/EnvironmentConfigPopupRequestedEvent.hpp"
#include "ui/EnvironmentDataChangedEvent.hpp"
#include "ui/EnvironmentDataRequestedEvent.hpp"
#include "ui/MaterialPopupRequestedEvent.hpp"
#include "ui/MaterialPopupChangedEvent.hpp"


using AppEventVariant = std::variant<
	//Flow
	SceneDataRequestedEvent,
	SceneDataChangedEvent,
//	AppShutdownEvent,

	//State
	AppModeChangedEvent,
	AppModeCycleRequestedEvent,
	AppModeSetRequestedEvent,
	CameraModeChangedEvent,
	SceneObjectChangedEvent,
	WindowSizeChangedEvent,

	//ui
	CreatePopupChangedEvent,
	CreatePopupRequestedEvent,
	EnvironmentConfigPopupChangedEvent, //창 열기
	EnvironmentConfigPopupRequestedEvent,
	EnvironmentDataChangedEvent,
	EnvironmentDataRequestedEvent,
	MaterialPopupRequestedEvent,
	MaterialPopupChangedEvent
>;
