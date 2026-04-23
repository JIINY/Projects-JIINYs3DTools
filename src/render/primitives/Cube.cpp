#include "Cube.hpp"
#include <vector>
#include "common/Math.hpp"
using namespace std;
using namespace Math;

namespace Render::Primitives
{
    void Cube::build(float width, float depth, float height, int segmentsX, int segmentsY, int segmentsZ)
    {
        if (width < 0.01f) { width = 0.01f; }
        if (depth < 0.01f) { depth = 0.01f; }
        if (height < 0.01f) { height = 0.01f; }
        if (segmentsX < 1) { segmentsX = 1; }
        if (segmentsY < 1) { segmentsY = 1; }
        if (segmentsZ < 1) { segmentsZ = 1; }
        vertices_.clear();
        indices_.clear();
            
        size_t vertCount = 2 * ((segmentsX + 1) * (segmentsY + 1) + (segmentsZ + 1) * (segmentsY + 1) + (segmentsX + 1) * (segmentsZ + 1));
        size_t indCount = 2 * (segmentsX * segmentsY + segmentsZ * segmentsY + segmentsX * segmentsZ) * 6;
        vertices_.reserve(vertCount);
        indices_.reserve(indCount);

        float halfW = width * 0.5f;
        float halfH = height * 0.5f;
        float halfD = depth * 0.5f;
        Vec4 color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);

        auto buildFace = [&](Vec3 normal, Vec3 uDir, Vec3 vDir, float sizeU, float sizeV, int segU, int segV, float offset)
        {
            int baseIndex = (int)vertices_.size();
            float stepU = sizeU / segU;
            float stepV = sizeV / segV;
            float halfSizeU = sizeU * 0.5f;
            float halfSizeV = sizeV * 0.5f;

            for (int i = 0; i <= segV; ++i)
            {
                float vPos = i * stepV - halfSizeV;
                float vCoord = (float)i / segV;

                for (int j = 0; j <= segU; ++j)
                {
                    float uPos = j * stepU - halfSizeU;
                    float uCoord = (float)j / segU;

                    Vec3 pos = normal * offset + uDir * uPos + vDir * vPos;
                    Vec2 uv(uCoord, vCoord);

                    vertices_.push_back({ pos, normal, uv, color });
                }
            }

            int rowCount = segU + 1;
            for (int i = 0; i < segV; ++i)
            {
                for (int j = 0; j < segU; ++j)
                {
                    int topLeft = baseIndex + i * rowCount + j;
                    int bottomLeft = baseIndex + (i + 1) * rowCount + j;

                    indices_.push_back(topLeft);
                    indices_.push_back(bottomLeft + 1);
                    indices_.push_back(bottomLeft);

                    indices_.push_back(topLeft);
                    indices_.push_back(topLeft + 1);
                    indices_.push_back(bottomLeft + 1);
                }
            }
        };

        buildFace(Vec3(0, 0, 1), Vec3(1, 0, 0), Vec3(0, 1, 0), width, height, segmentsX, segmentsY, halfD);
        buildFace(Vec3(0, 0, -1), Vec3(-1, 0, 0), Vec3(0, 1, 0), width, height, segmentsX, segmentsY, halfD);
        buildFace(Vec3(1, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0), depth, height, segmentsZ, segmentsY, halfW);
        buildFace(Vec3(-1, 0, 0), Vec3(0, 0, 1), Vec3(0, 1, 0), depth, height, segmentsZ, segmentsY, halfW);
        buildFace(Vec3(0, 1, 0), Vec3(1, 0, 0), Vec3(0, 0, -1), width, depth, segmentsX, segmentsZ, halfH);
        buildFace(Vec3(0, -1, 0), Vec3(1, 0, 0), Vec3(0, 0, 1), width, depth, segmentsX, segmentsZ, halfH);
    }
}
