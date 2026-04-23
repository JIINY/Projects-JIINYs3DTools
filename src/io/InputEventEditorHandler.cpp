#include "InputEventEditorHandler.hpp"
#include "Windows.h"
#include "common/Math.hpp"
#include "event/InputEventType.hpp"
#include "event/editorEvent/EditorEventPublisher.hpp"
using namespace std;
using namespace Math;


void InputEventEditorHandler::onInputEvent(const InputEvent& event, Ray mouseRay)
{
    visit([&](auto&& ev)
        {
            using T = decay_t<decltype(ev)>;
            if constexpr (is_same_v<T, KeyDownEvent>) { onKeyDown(ev, mouseRay); }
            else if constexpr (is_same_v<T, KeyHoldEvent>) { onKeyHold(ev, mouseRay); }
            else if constexpr (is_same_v<T, KeyUpEvent>) { onKeyUp(ev, mouseRay); }
            else if constexpr (is_same_v<T, MouseDownEvent>) { onMouseDown(ev, mouseRay); }
            else if constexpr (is_same_v<T, MouseHoldEvent>) { onMouseHold(ev, mouseRay); }
            else if constexpr (is_same_v<T, MouseUpEvent>) { onMouseUp(ev, mouseRay); }
            else if constexpr (is_same_v<T, MouseWheelEvent>) { onMouseWheel(ev, mouseRay); }
            else if constexpr (is_same_v<T, MouseMoveEvent>) { onMouseMove(ev, mouseRay); }
        }, event);
}

void InputEventEditorHandler::onKeyDown(const KeyDownEvent& event, Math::Ray mouseRay)
{
    EditorEventPublisher::get().publish(KeyDownEditorEvent{ event.keyCode_, event.isCtrl_, event.isAlt_, event.isShift_ });
}

void InputEventEditorHandler::onKeyHold(const KeyHoldEvent& event, Math::Ray mouseRay)
{
    EditorEventPublisher::get().publish(KeyHoldEditorEvent{ event.keyCode_, event.isCtrl_, event.isAlt_, event.isShift_ });
}

void InputEventEditorHandler::onKeyUp(const KeyUpEvent& event, Math::Ray mouseRay)
{
    EditorEventPublisher::get().publish(KeyUpEditorEvent{ event.keyCode_, event.isCtrl_, event.isAlt_, event.isShift_ });
}

void InputEventEditorHandler::onMouseDown(const MouseDownEvent& event, Math::Ray mouseRay)
{
    EditorEventPublisher::get().publish(MouseDownEditorEvent{ event.button_, event.pos_, mouseRay, event.isCtrl_, event.isAlt_, event.isShift_ });
}

void InputEventEditorHandler::onMouseHold(const MouseHoldEvent& event, Math::Ray mouseRay)
{
    EditorEventPublisher::get().publish(MouseHoldEditorEvent{ event.button_, event.deltaX_, event.deltaY_, event.pos_, mouseRay, event.isCtrl_, event.isAlt_, event.isShift_ });
}

void InputEventEditorHandler::onMouseUp(const MouseUpEvent& event, Math::Ray mouseRay)
{
    EditorEventPublisher::get().publish(MouseUpEditorEvent{ event.button_, event.pos_, mouseRay, event.isCtrl_, event.isAlt_, event.isShift_ });
}

void InputEventEditorHandler::onMouseWheel(const MouseWheelEvent& event, Math::Ray mouseRay)
{
    EditorEventPublisher::get().publish(MouseWheelEditorEvent{ event.delta_, event.pos_, mouseRay, event.isCtrl_, event.isAlt_, event.isShift_ });
}

void InputEventEditorHandler::onMouseMove(const MouseMoveEvent& event, Math::Ray mouseRay)
{
    EditorEventPublisher::get().publish(MouseMoveEditorEvent{ event.deltaX_, event.deltaY_, event.pos_, mouseRay, event.isCtrl_, event.isAlt_, event.isShift_ });
}
