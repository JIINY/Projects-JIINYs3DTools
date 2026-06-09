#pragma once
#include <memory>
#include <map>

class PassiveObjectCoordinator;


namespace Create
{
    class CreatePrimitive
    {
    public:
        bool initialize(PassiveObjectCoordinator* passiveObjCoord);
        void draw();


    private:
        PassiveObjectCoordinator* passiveObjCoord_ = nullptr;

        //void drawCubeSettings();
        //void drawCylinderSettings();
        //void drawPlaneSettings();
        //void drawSphereSettings();
        //void drawCapsuleSettings();
        //void drawTeapotSettings();
        //void drawSettings();
    };
}
