#pragma once
#include "../Mesh.hpp"

namespace Render::Primitives
{
    class Capsule : public Mesh 
    {
    public:
        Capsule(float radius = 0.5f, float height = 2.0f, float heightHemi = 0.5f, int segmentsX = 8, int segmentsY = 1, int segmentsHemi = 3)
        {
            build(radius, height, heightHemi, segmentsX, segmentsY, segmentsHemi);
        }
        virtual ~Capsule() = default;

    private:
        void build(float radius, float height, float heightHemi, int segmentsX, int segmentsY, int segmentsHemi);
    };
}
