#include "Cylinder.hpp"
#include <vector>
#include <cmath>
#include "../Mesh.hpp"
using namespace std;
using namespace Math;


namespace Render::Primitives
{
    void Cylinder::build(float radiusTop, float radiusBot, float height, int segmentsX, int segmentsY) 
    {
        if (radiusTop < 0.01f) { radiusTop = 0.01f; }
        if (radiusBot < 0.01f) { radiusBot = 0.01f; }
        if (height < 0.01f) { height = 0.01f; }
        if (segmentsX < 3) { segmentsX = 3; }
        if (segmentsY < 1) { segmentsY = 1; }

        vertices_.clear();
        indices_.clear();

        size_t sideVerts = (segmentsX + 1) * (segmentsY + 1);
        size_t capVerts = segmentsX + 2;
        vertices_.reserve(sideVerts + 2 * capVerts);
        size_t sideIndices = segmentsX * segmentsY * 6;
        size_t capIndices = segmentsX * 3;
        indices_.reserve(sideIndices + 2 * capIndices);

        float stackHeight = height / segmentsY;
        float radiusStep = (radiusTop - radiusBot) / segmentsY;
        float ringCount = segmentsY + 1;
        Vec4 color(1.0f, 1.0f, 1.0f, 1.0f);

        //옆면 정점
        for (int i = 0; i < ringCount; ++i) 
        {
            float y = -0.5f * height + i * stackHeight;
            float r = radiusBot + i * radiusStep;
            float v = (float)i / segmentsY;

            for (int j = 0; j <= segmentsX; ++j) 
            {
                float phi = j * 2.0f * PI / segmentsX + PI * 0.25f;
                float u = (float)j / segmentsX;

                float cosTheta = cos(phi);
                float sinTheta = sin(phi);

                //위치
                Vec3 pos(r * cosTheta, y, r * sinTheta);

                //법선 (원기둥은 cos, 0, sin)
                Vec3 normal(cosTheta, 0.0f, sinTheta);

                vertices_.push_back({ pos, normal, Vec2(u, 1.0f - v), color });
            }
        }

        //옆면 인덱스
        int ringVertexCount = segmentsX + 1;
        for (int i = 0; i < segmentsY; ++i) 
        {
            for (int j = 0; j < segmentsX; ++j) 
            {
                int current = i * ringVertexCount + j;
                int next = (i + 1) * ringVertexCount + j;

                indices_.push_back(current);
                indices_.push_back(next + 1);
                indices_.push_back(current + 1);

                indices_.push_back(current);
                indices_.push_back(next);
                indices_.push_back(next + 1);
            }
        }

        //뚜껑
        auto buildCap = [&](float radius, float y, float normalY, bool isTop)
        {
            int baseIndex = (int)vertices_.size();
            int centerIndex = baseIndex;

            //중심점
            vertices_.push_back({ Vec3(0, y, 0), Vec3(0, normalY, 0), Vec2(0.5f, 0.5f), color });

            //원주점
            for (int j = 0; j <= segmentsX; ++j)
            {
                float phi = j * 2.0f * PI / segmentsX + PI * 0.25f;
                float x = radius * cos(phi);
                float z = radius * sin(phi);

                float u = x / height + 0.5f;
                float v = z / height + 0.5f;

                vertices_.push_back({ Vec3(x, y, z), Vec3(0, normalY, 0), Vec2(u, v), color });
            }

            //인덱스
            for (int j = 0; j < segmentsX; ++j)
            {
                if (isTop)
                {
                    indices_.push_back(centerIndex);
                    indices_.push_back(baseIndex + j + 2);
                    indices_.push_back(baseIndex + j + 1);
                }
                else
                {
                    indices_.push_back(centerIndex);
                    indices_.push_back(baseIndex + j + 1);
                    indices_.push_back(baseIndex + j + 2);
                }
            }
        };

        buildCap(radiusTop, height * 0.5f, 1.0f, true);
        buildCap(radiusBot, -height * 0.5f, -1.0f, false);
    }
}
