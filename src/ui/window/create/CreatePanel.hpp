#pragma once
#include <memory>

class PassiveObjectCoordinator;
namespace Create { class CreatePrimitive; }


namespace Create 
{
    class CreatePanel
    {
    public:
        CreatePanel();
        ~CreatePanel();

        bool initialize(PassiveObjectCoordinator* passiveObjCoord);
        void draw(bool isVisible);

    private:
        std::unique_ptr<CreatePrimitive> primitive_;
    };
}
