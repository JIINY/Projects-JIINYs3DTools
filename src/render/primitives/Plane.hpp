#pragma once
#include "../Mesh.hpp"


namespace Render::Primitives
{
    class Plane : public Mesh
    {
    public:
        Plane(float width = 5.0f, float depth = 5.0, int segmentsX = 1, int segmentsY = 1) { build(width, depth, segmentsX, segmentsY); }
        virtual ~Plane() = default;

    private:
        void build(float width, float depth, int segmentsX, int segmentsY);
    };
}
