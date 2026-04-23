#include "Sphere.hpp"
#include <vector>
#include "common/Math.hpp"
using namespace std;
using namespace Math;


namespace Render::Primitives
{
    void Sphere::build(float radius, int segmentsX, int segmentsY) 
    {
        if (radius < 0.01f) { radius = 0.01f; }
        if (segmentsX < 3) { segmentsX = 3; }
        if (segmentsY < 1) { segmentsY = 1; }

        vertices_.clear();
        indices_.clear();
        vertices_.reserve((segmentsX + 1) * (segmentsY + 1));
        indices_.reserve(segmentsX * segmentsY * 6);

        Vec4 color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
        for (int i = 0; i <= segmentsY; ++i)
        {
            float phi = i * PI / segmentsY;

            for (int j = 0; j <= segmentsX; ++j) 
            {
                float theta = j * TWO_PI / segmentsX;

                Vec3 pos = getSphericalCoord(radius, theta, phi);
                Vec3 normal = normalize(pos);
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
                indices_.push_back(topLeft + 1);
                indices_.push_back(bottomLeft + 1);

                indices_.push_back(topLeft);
                indices_.push_back(bottomLeft + 1);
                indices_.push_back(bottomLeft);
            }
        }
    }
}
