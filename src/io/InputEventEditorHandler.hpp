#pragma once
#include "common/Math.hpp"
#include "event/InputEventType.hpp"

class InputEventEditorHandler
{
public:
    void onInputEvent(const InputEvent& event, Math::Ray mouseRay);

private:
    void onKeyDown(const KeyDownEvent& event, Math::Ray mouseRay);
    void onKeyHold(const KeyHoldEvent& event, Math::Ray mouseRay);
    void onKeyUp(const KeyUpEvent& event, Math::Ray mouseRay);
    void onMouseDown(const MouseDownEvent& event, Math::Ray mouseRay);
    void onMouseHold(const MouseHoldEvent& event, Math::Ray mouseRay);
    void onMouseUp(const MouseUpEvent& event, Math::Ray mouseRay);
    void onMouseWheel(const MouseWheelEvent& event, Math::Ray mouseRay);
    void onMouseMove(const MouseMoveEvent& event, Math::Ray mouseRay);
};
