#pragma once
#include <variant>
#include "io/KeyboardEvent.hpp"
#include "io/MouseEvent.hpp"
#include "selection/SelectionChangeEvent.hpp"
#include "selection/SelectionModeEvent.hpp"
#include "transform/GizmoModeRequestedEvent.hpp"
#include "transform/GizmoDragEvent.hpp"

using EditorEventVariant = std::variant<
	KeyDownEditorEvent,
	KeyHoldEditorEvent,
	KeyUpEditorEvent,
	MouseDownEditorEvent,
	MouseHoldEditorEvent,
	MouseUpEditorEvent,
	MouseWheelEditorEvent,
	MouseMoveEditorEvent,

	SelectionRequestedEvent,
	SelectionDragRequestedEvent,
	SelectionChangedEvent,
	SelectionModeChangedEvent,

	GizmoModeRequestedEvent,
	GizmoDragEvent
>;
