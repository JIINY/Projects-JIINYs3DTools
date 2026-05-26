#pragma once
#include <variant>
#include "material/MaterialTargetChangedEvent.hpp"
#include "viewport/GridChangedEvent.hpp"
#include "viewport/GridRequestedEvent.hpp"
#include "viewport/CameraInfoChangedEvent.hpp"
#include "viewport/CameraInfoRequestedEvent.hpp"
#include "viewport/WireframeChangedEvent.hpp"
#include "viewport/SolidChangedEvent.hpp"


using UIEventVariant = std::variant<
	MaterialTargetChangedEvent,
	GridChangedEvent,
	GridRequestedEvent,
	CameraInfoChangedEvent,
	CameraInfoRequestedEvent,
	WireframeChangedEvent,
	SolidChangedEvent
>;
