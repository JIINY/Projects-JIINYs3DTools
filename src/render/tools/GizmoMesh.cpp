#include "GizmoMesh.hpp"
#include <DirectXMath.h>
#include "../primitives/Cylinder.hpp"
#include "../primitives/Torus.hpp"
using namespace std;
using namespace DirectX;
using namespace Render::Primitives;


namespace Render::Tools 
{
    bool GizmoMesh::initialize(ID3D11Device* device, const GizmoData& data) 
    {
        vertices_.clear();
        indices_.clear();
        vertexBuffer_ = nullptr;
        indexBuffer_ = nullptr;

        switch (data.shape_)
        {
        case GizmoShape::Arrow:
        {
            Cylinder body(data.bodyRadius_, data.bodyRadius_, data.bodyLength_, 8);
            mergeGeometry(body, data.bodyLength_ * 0.5f);

            Cylinder head(0.0f, data.headRadius_, data.headLength_, data.slices_);
            mergeGeometry(head, data.bodyLength_ + (data.headLength_ * 0.5f));
            break;
        }
        case GizmoShape::CubeBar:
        {
            Cylinder body(data.bodyRadius_, data.bodyRadius_, data.bodyLength_, 8);
            mergeGeometry(body, data.bodyLength_ * 0.5f);

            Cylinder head(data.headRadius_, data.headRadius_, data.headLength_, 4);
            mergeGeometry(head, data.bodyLength_ + (data.headLength_ * 0.5f));
            break;
        }
        case GizmoShape::Ring:
        {
            Torus ring(data.radius_, data.thickness_, data.segmentsX_, data.segmentsY_);
            mergeGeometry(ring, 0.0f);
            break;
        }
        default: return false;
        }

        Mesh::initialize(device);
        return true;
    }

    void GizmoMesh::mergeGeometry(const Mesh& mesh, float yOffset) 
    {
        unsigned int indexOffset = static_cast<unsigned int>(vertices_.size());

        const auto& srcVerts = mesh.getVertices();
        for (const auto& v : srcVerts) 
        {
            Vertex newV = v;
            newV.position.y += yOffset;

            this->vertices_.push_back(newV);
        }

        const auto& srcIndices = mesh.getIndices();
        for (unsigned int index : srcIndices) 
        {
            this->indices_.push_back(index + indexOffset);
        }
    }
}
