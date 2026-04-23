#pragma once
#include "../Mesh.hpp"

namespace Render::Primitives
{
    class Cube : public Mesh 
    {
    public:
        Cube(float width = 1.0f, float depth = 1.0f, float height = 1.0f, int segmentsX = 1, int segmentsY = 1, int segmentsZ = 1) 
        { 
            build(width, depth, height, segmentsX, segmentsY, segmentsZ); 
        }
        virtual ~Cube() = default;

    private:
        void build(float width, float depth, float height, int segmentsX, int segmentsY, int segmentsZ);
    };
}
