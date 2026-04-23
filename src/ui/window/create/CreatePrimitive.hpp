#pragma once
#include <memory>
#include <map>
#include "render/primitives/PrimitivesType.hpp"
#include "core/coordinator/scene/PassiveObjectCoordinator.hpp"


namespace Create
{
    class CreatePrimitive
    {
    public:
        bool initialize(PassiveObjectCoordinator* passiveObjCoord);
        void draw();


    private:
        PassiveObjectCoordinator* passiveObjCoord_ = nullptr;

        Render::Primitives::PrimitiveData setDefaultData(Render::Primitives::PrimitiveType type);

        //void drawCubeSettings();
        //void drawCylinderSettings();
        //void drawPlaneSettings();
        //void drawSphereSettings();
        //void drawCapsuleSettings();
        //void drawTeapotSettings();
        //void drawSettings();
    };
}
