#pragma once
#include "IMesh.hpp"
#include <vector>
#include <wrl/client.h>
#include "../common/Math.hpp"

namespace Render 
{
    struct Vertex 
    {
        Math::Vec3 position;
        Math::Vec3 normal;
        Math::Vec2 texCoords;
        Math::Vec4 color;
    };


    class Mesh : public IMesh 
    {
    public:
        virtual ~Mesh() = default;

        void initialize(ID3D11Device* device) override;
        void draw(ID3D11DeviceContext* context) const override;

        const std::vector<Vertex>& getVertices() const { return vertices_; }
        const std::vector<unsigned int>& getIndices() const { return indices_; } //선긋기 순서표

        unsigned int getVertexCount() const override { return static_cast<unsigned int>(vertices_.size()); }
        unsigned int getIndexCount() const override { return static_cast<unsigned int>(indices_.size()); }


    protected:
        std::vector<Vertex> vertices_;
        std::vector<unsigned int> indices_;
        D3D11_PRIMITIVE_TOPOLOGY topology_ = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

        Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer_;
        Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer_;
    };
}
