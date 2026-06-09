#include "GizmoObject.hpp"
#include <memory>
#include <string>
#include <algorithm>
#include <DirectXCollision.h>
#include "core/manager/resources/MaterialManager.hpp"
#include "../shaders/Unlit_MaterialColor/Unlit_Preset_MaterialColor.hpp"
#include "common/Math.hpp"
#include "GizmoMesh.hpp"
#include "../RenderObject.hpp"

#include "common/DebugLog.hpp"
using namespace std;
using namespace DirectX;
using namespace Math;


namespace Render::Tools 
{
    void GizmoObject::initialize(GizmoObjectContext context, Math::Axis axis)
    {
        data_ = context.data;
        axis_ = axis;

        auto mesh = std::make_shared<GizmoMesh>();
        mesh->initialize(context.device, context.data);

        auto material = context.matManager->createMaterial(L"Unlit_MaterialColor");
        material->setColor("MaterialColor", AxisInfo::GetColor(axis));
        
        RenderObject::initialize(context.device, mesh, material);
        setRenderQueue(static_cast<int>(Render::RenderQueue::Overlay));
        setCastShadow(false);
        setReceiveShadow(false);

        buildBoundingBox(data_, axis_);
    }

    void GizmoObject::buildBoundingBox(const GizmoData& data, Axis axis) 
    {
        const float pickPadding = 0.15f;
        switch (data.shape_) 
        {
        case GizmoShape::Arrow:
        case GizmoShape::CubeBar:
        {
            float halfLength = (data.bodyLength_ + data.headLength_) * 0.5f;
            float radius = ((std::max)(data.bodyRadius_, data.headRadius_) + pickPadding) * 0.5f;
            halfLength_ = halfLength;

            boundingBox_.Center = XMFLOAT3(0.0f, halfLength, 0.0f);
            boundingBox_.Extents = XMFLOAT3(radius, halfLength, radius);
            break;
        }
        case GizmoShape::Ring:
        {
            float radiusExtent = data.radius_ + data.thickness_ + pickPadding;
            float thicknessExtent = data.thickness_ + pickPadding - 0.05f;
            boundingBox_.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
            boundingBox_.Extents = XMFLOAT3(radiusExtent, thicknessExtent, radiusExtent);
            break;
        }
        }
    }

    bool GizmoObject::intersects(const Math::Vec3& rayOrigin, const Math::Vec3& rayDir, float& outDist)
    {
        if (data_.shape_ == GizmoShape::Ring)
        {
            return intersectsRing(rayOrigin, rayDir, outDist);
        }

        XMMATRIX world = XMLoadFloat4x4(&worldMatForCollider_);
        BoundingBox worldBB;
        boundingBox_.Transform(worldBB, world);

        //충돌 테스트
        XMVECTOR worldOrigin = XMLoadFloat3(&rayOrigin);
        XMVECTOR worldDir = XMLoadFloat3(&rayDir);
        worldDir = XMVector3Normalize(worldDir);

        return worldBB.Intersects(worldOrigin, worldDir, outDist);
    }

    void GizmoObject::setWorldMatrix(const DirectX::XMMATRIX& meshRot, const DirectX::XMMATRIX& colliderRot)
    { 
        XMStoreFloat4x4(&worldMatForShader_, meshRot);
        XMStoreFloat4x4(&worldMatForCollider_, colliderRot);
    }

    bool GizmoObject::intersectsRing(const Math::Vec3& rayOrigin, const Math::Vec3& rayDir, float& outDist) const
    {
        XMMATRIX world = XMLoadFloat4x4(&worldMatForCollider_);
        XMMATRIX invWorld = XMMatrixInverse(nullptr, world);

        //ray를 로컬 공간으로 변환
        XMVECTOR localOrigin = XMVector3TransformCoord(XMLoadFloat3(&rayOrigin), invWorld);
        XMVECTOR localDir = XMVector3TransformNormal(XMVector3Normalize(XMLoadFloat3(&rayDir)), invWorld);

        float radius = data_.radius_;
        float pickPadding = 0.15f;
        float thickness = data_.thickness_ + pickPadding;
        int segments = data_.segmentsX_;

        bool hit = false;
        float minDist = FLT_MAX;

        for (int i = 0; i < segments; ++i)
        {
            float theta = (float)i / segments * 2.0f * XM_PI;
            XMVECTOR center = XMVectorSet(radius * cosf(theta), 0.0f, radius * sinf(theta), 0.0f);

            BoundingSphere sphere;
            XMStoreFloat3(&sphere.Center, center);
            sphere.Radius = thickness;

            float dist = 0.0f;
            if (sphere.Intersects(localOrigin, localDir, dist))
            {
                if (dist < minDist)
                {
                    minDist = dist;
                    hit = true;
                }
            }
        }

        if (hit) { outDist = minDist; }
        return hit;
    }
}
