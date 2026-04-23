#include "Capsule.hpp"
#include <vector>
#include <cmath>

#include "common/DebugLog.hpp"
using namespace std;
using namespace Math;

namespace Render::Primitives
{
    void Capsule::build(float radius, float height, float heightHemi, int segmentsX, int segmentsY, int segmentsHemi) 
    {
        if (radius < 0.01f) { radius = 0.01f; }
        if (height < 0.01f) { height = 0.01f; }
        if (heightHemi < 0.01f) { heightHemi = 0.01f; }
        if (segmentsX < 3) { segmentsX = 3; }
        if (segmentsY < 1) { segmentsY = 1; }
        if (segmentsHemi < 1) { segmentsHemi = 1; }

        int rowCount = segmentsX + 1;
        int totalStack = segmentsY + segmentsHemi * 2;
        Vec4 color(1.0f, 1.0f, 1.0f, 1.0f);

        vertices_.clear();
        indices_.clear();
        vertices_.reserve(rowCount * (totalStack + 1));
        indices_.reserve(totalStack * segmentsX * 6);

        for (int i = 0; i <= totalStack; ++i)
        {
            float y = 0.0f;
            float currentRadius = radius;

            Vec3 normalCenter(0.0f, 0.0f, 0.0f);
            bool isBody = false;

            if (i <= segmentsHemi) // 상단 반구
            {
                float phi = (PI * 0.5f) * (1.0f - (float)i / segmentsHemi);
                y = (height * 0.5f) + (radius * sin(phi) * (heightHemi / radius));
                currentRadius = radius * cos(phi);
                normalCenter = Vec3(0.0f, height * 0.5f, 0.0f);
            }
            else if (i <= segmentsHemi + segmentsY) // 몸통
            {
                float localV = (float)(i - segmentsHemi) / segmentsY;
                y = (height * 0.5f) - (localV * height);
                currentRadius = radius;

                isBody = true;
            }
            else // 하단 반구
            {
                float localI = (float)(i - (segmentsHemi + segmentsY)) / segmentsHemi;
                float phi = (PI * 0.5f) * localI;
                y = (-height * 0.5f) - (radius * sin(phi) * (heightHemi / radius));
                currentRadius = radius * cos(phi);
                normalCenter = Vec3(0.0f, -height * 0.5f, 0.0f);
            }

            for (int j = 0; j <= segmentsX; ++j)
            {
                float u = (float)j / segmentsX;
                float theta = u * TWO_PI;

                Vec3 pos(currentRadius * cos(theta), y, currentRadius * sin(theta));
                Vec3 normal;

                if (isBody) 
                {
                    normal = Vec3(cos(theta), 0.0f, sin(theta));
                }
                else 
                {
                    normal = normalize(pos - normalCenter);
                }
                    
                Vec2 uv = Vec2(u, (float)i / totalStack);
                vertices_.push_back({ pos, normal, uv, color });
            }
        }

        //인덱스
        for (int i = 0; i < totalStack; ++i)
        {
            for (int j = 0; j < segmentsX; ++j)
            {
                int current = i * rowCount + j;
                int next = (i + 1) * rowCount + j;

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
