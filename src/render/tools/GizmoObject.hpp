#pragma once
#include <memory>
#include <DirectXCollision.h>
#include <string>
#include "common/Math.hpp"
#include "GizmoMesh.hpp"
#include "../RenderObject.hpp"
#include "selection/Selectable.hpp"

class MaterialManager;


namespace Render::Tools 
{
    struct GizmoObjectContext
    {
        ID3D11Device* device = nullptr;
        MaterialManager* matManager = nullptr;
        GizmoData data{};
    };

    class GizmoObject : public Render::RenderObject, public Selection::Selectable
    {
    public:
        virtual ~GizmoObject() = default;

        void initialize(GizmoObjectContext context, Math::Axis);
        
        virtual bool intersects(const Math::Vec3& rayOrigin, const Math::Vec3& rayDir, float& outDist) override;

        void setWorldMatrix(const DirectX::XMMATRIX& meshRot, const DirectX::XMMATRIX& colliderRot);
        const DirectX::BoundingBox& getBoundingBox() const { return boundingBox_; }
        const GizmoData& getData() const { return data_; }
        float getHalfLength() const { return halfLength_; }
        virtual std::string getName() const override { return "GizmoHandle"; }

    private:
        GizmoData data_;
        DirectX::BoundingBox boundingBox_;
        Math::Mat4 worldMatForCollider_;
        Math::Axis axis_ = Math::Axis::Count;
        float halfLength_ = 0.0f;
        MaterialManager* materialManager_ = nullptr;

        void buildBoundingBox(const GizmoData& data, Math::Axis);
        bool intersectsRing(const Math::Vec3& rayOrigin, const Math::Vec3& rayDir, float& outDist) const;
    };
}
