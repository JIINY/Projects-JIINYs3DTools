#include "GridMesh.hpp"
#include "../Mesh.hpp"
#include "common/Math.hpp"
using namespace std;
using namespace Math;


namespace Render::Tools
{
    GridMesh::GridMesh() 
    {
        topology_ = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
    }

    void GridMesh::rebuild(ID3D11Device* device, int gridSize, float gridSpacing) 
    {
        vertices_.clear();
        indices_.clear();
        vertexBuffer_ = nullptr;
        indexBuffer_ = nullptr;

        const float maxPos = gridSize * gridSpacing;

        auto addVertex = [&](float x, float y, float z, const Vec4& color) {
            Vertex v;
            v.position = Vec3(x, y, z);
            v.normal = Vec3(0, 1, 0);
            v.texCoords = Vec2(0, 0);
            v.color = color;
            vertices_.push_back(v);
            indices_.push_back(static_cast<unsigned int>(vertices_.size() - 1));
        };

        for (int i = -gridSize; i <= gridSize; ++i) //세로선

        {
            float pos = i * gridSpacing;
            const Vec4& c = (i == 0) ? axisColor_ : lineColor_;
            addVertex(pos, 0.0f, -maxPos, c);
            addVertex(pos, 0.0f, maxPos, c);
        }

        for (int i = -gridSize; i <= gridSize; ++i) 
        {
            float pos = i * gridSpacing;
            const Vec4& c = (i == 0) ? axisColor_ : lineColor_;
            addVertex(-maxPos, 0.0f, pos, c);
            addVertex(maxPos, 0.0f, pos, c);
        }

        initialize(device);
    }
}
