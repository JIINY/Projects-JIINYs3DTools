#pragma once
#include "CreatePrimitive.hpp"

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
