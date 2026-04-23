#pragma once
#include "../Mesh.hpp"


namespace Render::Primitives 
{
    class Torus : public Mesh 
    {
    public:
        Torus(float radius = 1.0f, float thickness = 0.1f, int segmentsX = 32, int segmentsY = 16) { build(radius, thickness, segmentsX, segmentsY); }
        virtual ~Torus() = default;


    private:
        void build(float radius, float thickness, int segmentsX, int segmentsY);
    };
}
