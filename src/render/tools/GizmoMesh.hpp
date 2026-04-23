#pragma once
#include "../Mesh.hpp"


namespace Render::Tools
{
    enum class GizmoShape 
    {
        Arrow,
        Ring,
        CubeBar,
        Count
    };

    struct GizmoData 
    {
        GizmoShape shape_ = GizmoShape::Arrow;
        float length_ = 1.5f;
        float headRadius_ = 0.12f;
        float bodyRadius_ = 0.03f;
        int slices_ = 12;

        float headLength_;
        float bodyLength_;

        float radius_ = 1.0f;
        float thickness_ = 0.015f;
        int segmentsX_ = 32;
        int segmentsY_ = 6;

        GizmoData(GizmoShape shape = GizmoShape::Arrow) : shape_(shape) 
        {
            headLength_ = length_ * 0.2f;
            bodyLength_ = length_ - headLength_;
            if (shape_ != GizmoShape::Ring) { thickness_ = bodyRadius_; }
            if (shape_ == GizmoShape::CubeBar)
            {
                headRadius_ = 0.14f;
                headLength_ = 0.22f;
                bodyLength_ = length_ - headLength_;
            }
        }
    };

    class GizmoMesh : public Mesh
    {
    public:
        GizmoMesh() { topology_ = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; }
        virtual ~GizmoMesh() = default;

        bool initialize(ID3D11Device* device, const GizmoData& data);


    private:
        void mergeGeometry(const Mesh& mesh, float yOffset);
    };
}
