#include "Torus.hpp"
#include <vector>
#include <cmath>
#include "../Mesh.hpp"
using namespace std;
using namespace Math;


namespace Render::Primitives 
{
    void Torus::build(float radius, float thickness, int segmentsX, int segmentsY) 
    {
        if (radius < 0.01f) { radius = 0.01f; }
        if (thickness < 0.001f) { thickness = 0.001f; }
        if (segmentsX < 3) { segmentsX = 3; }
        if (segmentsY < 3) { segmentsY = 3; }

        vertices_.clear();
        indices_.clear();

        size_t numVert = (segmentsX + 1) * (segmentsY + 1);
        vertices_.reserve(numVert);
        size_t numIndices = segmentsX * segmentsY * 6;
        indices_.reserve(numIndices);

        Vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
        for (int i = 0; i <= segmentsX; ++i) 
        {
            float theta = (float)i / segmentsX * 2.0f * PI;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            for (int j = 0; j <= segmentsY; ++j) 
            {
                float phi = (float)j / segmentsY * 2.0f * PI;
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);

                float u = (float)i / segmentsX;
                float v = (float)j / segmentsY;
                Vec2 uv(u, v);

                float dist = radius + thickness * cosPhi;
                float x = dist * cosTheta;
                float y = thickness * sinPhi;
                float z = dist * sinTheta;
                Vec3 pos(x, y, z);

                float centerX = radius * cosTheta;
                float centerZ = radius * sinTheta;

                Vec3 normal(x - centerX, y, z - centerZ);
                normal = normalize(normal);

                vertices_.push_back({pos, normal, uv, color});
            }
        }

        int ringVert = segmentsY + 1;
        for (int i = 0; i < segmentsX; ++i) 
        {
            for (int j = 0; j < segmentsY; ++j) 
            {
                int current = i * ringVert + j;
                int next = (i + 1) * ringVert + j;

                indices_.push_back(current);
                indices_.push_back(current + 1);
                indices_.push_back(next + 1);

                indices_.push_back(current);
                indices_.push_back(next + 1);
                indices_.push_back(next);
            }
        }
    }
}
