#pragma once
#include <memory>
#include "event/appEvent/AppEventSubscriber.hpp"
#include "event/appEvent/ui/CreatePopupChangedEvent.hpp"
#include "CreatePrimitive.hpp"

class CommandStack;
class PassiveObjectCoordinator;


namespace Create 
{
    class CreatePanel
    {
    public:
        bool initialize(PassiveObjectCoordinator* passiveObjCoord);
        void draw(bool isVisible);

    private:
        Create::CreatePrimitive primitive_;
    };
}
