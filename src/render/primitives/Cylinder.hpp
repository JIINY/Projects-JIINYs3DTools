#pragma once
#include "../Mesh.hpp"

namespace Render::Primitives
{
    class Cylinder : public Mesh
    {
    public:
        Cylinder(float radiusTop = 0.5f, float radiusBot = 0.5f, float height = 2.0f, int segmentsX = 32, int segmentsY = 1) 
        {
            build(radiusTop, radiusBot, height, segmentsX, segmentsY);
        }
        virtual ~Cylinder() = default;

    private:
        void build(float radiusTop = 0.5f, float radius = 0.5f, float height = 2.0f, int segmentsX = 32, int segmentsY = 1);
    };
}
