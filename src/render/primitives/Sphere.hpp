#pragma once
#include "../Mesh.hpp"

namespace Render::Primitives
{
    class Sphere : public Mesh
    {
    public:
        Sphere(float radius = 0.5f, int segmentsX = 32, int segmentsY = 32) { build(radius, segmentsX, segmentsY); }
        virtual ~Sphere() = default;

    private:
        void build(float radius, int segmentsX, int segmentsY);
    };
}
