#include "Plane.hpp"
#include <vector>
#include "common/Math.hpp"
using namespace std;
using namespace Math;


namespace Render::Primitives
{
    void Plane::build(float width, float depth, int segmentsX, int segmentsY)
    {
        if (width < 0.01f) { width = 0.01f; }
        if (depth < 0.01f) { depth = 0.01f; }
        if (segmentsX < 1) { segmentsX = 1; }
        if (segmentsY < 1) { segmentsY = 1; }

        vertices_.clear();
        indices_.clear();
        vertices_.reserve((segmentsX + 1) * (segmentsY + 1));
        indices_.reserve(segmentsX * segmentsY * 6);

        float halfW = width * 0.5f;
        float halfD = depth * 0.5f;

        float dx = width / segmentsX;
        float dz = depth / segmentsY;
        Vec4 color(1.0f, 1.0f, 1.0f, 1.0f);

        for (int i = 0; i <= segmentsY; ++i) 
        {
            float z = halfD - i * dz;

            for (int j = 0; j <= segmentsX; ++j) 
            {
                float x = -halfW + j * dx;

                Vec3 pos(x, 0.0f, z);
                Vec3 normal(0.0f, 1.0f, 0.0f);
                Vec2 uv((float)j / segmentsX, (float)i / segmentsY);

                vertices_.push_back({ pos, normal, uv, color });
            }
        }

        int rowCount = segmentsX + 1;
        for (int i = 0; i < segmentsY; ++i) 
        {
            for (int j = 0; j < segmentsX; ++j) 
            {
                int topLeft = i * rowCount + j;
                int bottomLeft = (i + 1) * rowCount + j;

                indices_.push_back(topLeft);
                indices_.push_back(bottomLeft + 1);
                indices_.push_back(bottomLeft);

                indices_.push_back(topLeft);
                indices_.push_back(topLeft + 1);
                indices_.push_back(bottomLeft + 1);
            }
        }
    }
}
