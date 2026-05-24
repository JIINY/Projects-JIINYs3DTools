#include "Gizmo.hpp"
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>
#include <algorithm>
#include "common/Math.hpp"
#include "common/Mode.hpp"
#include "core/manager/resources/MaterialManager.hpp"
#include "render/RenderCommandQueue.hpp"
#include "render/tools/GizmoObject.hpp"

#include "common/DebugLog.hpp"
using namespace std;
using namespace DirectX;
using namespace Math;


namespace Render::Tools 
{
    Gizmo::Gizmo() : Objects::Entity(), x_(make_unique<Render::Tools::GizmoObject>()), 
        y_(make_unique<Render::Tools::GizmoObject>()), z_(make_unique<Render::Tools::GizmoObject>()) {}
    Gizmo::~Gizmo() = default;

    bool Gizmo::initialize(ID3D11Device* device, MaterialManager* matManager)
    {
        device_ = device;
        materialManager_ = matManager;

        setMode(TransformMode::Translate);
        return true;
    }

    void Gizmo::setMode(TransformMode mode)
    {
        currentMode_ = mode;
        hovered_ = Axis::Count;

        if (!device_ || mode == TransformMode::Count) { return; }

        auto getData = [](const unique_ptr<GizmoObject>& obj, GizmoShape shape) -> GizmoData
            {
                if (obj && obj->getData().shape_ == shape) { return obj->getData(); }
                return GizmoData(shape);
            };

        switch (mode) 
        {
        case TransformMode::Translate: currentShape_ = GizmoShape::Arrow; break;
        case TransformMode::Rotate: currentShape_ = GizmoShape::Ring; break;
        case TransformMode::Scale: currentShape_ = GizmoShape::CubeBar; break;
        default: currentShape_ = GizmoShape::Count; break;
        }

        GizmoObjectContext gizmoObjContext;
        gizmoObjContext.device = device_;
        gizmoObjContext.data = getData(x_, currentShape_);
        gizmoObjContext.matManager = materialManager_;
        x_->initialize(gizmoObjContext, Axis::X);
        y_->initialize(gizmoObjContext, Axis::Y);
        z_->initialize(gizmoObjContext, Axis::Z);
    }

    void Gizmo::update(const Math::Ray& mouseRay, const DirectX::XMMATRIX& targetWorld) 
    {
        if (currentShape_ == GizmoShape::Count || currentMode_ == TransformMode::Count) 
        {
            return;
        }

        XMVECTOR scale, rotQuat, trans;
        XMMatrixDecompose(&scale, &rotQuat, &trans, targetWorld);

        XMFLOAT3 pos;
        XMStoreFloat3(&pos, trans);
        this->setPosition(pos);
        
        XMMATRIX gizmoWorld = XMMatrixTranslationFromVector(trans);

        x_->setWorldMatrix(AxisInfo::GetRotationMX(Axis::X) * gizmoWorld, AxisInfo::GetRotationMX(Axis::X) * gizmoWorld);
        y_->setWorldMatrix(AxisInfo::GetRotationMX(Axis::Y) * gizmoWorld, AxisInfo::GetRotationMX(Axis::Y) * gizmoWorld);
        z_->setWorldMatrix(AxisInfo::GetRotationMX(Axis::Z) * gizmoWorld, AxisInfo::GetRotationMX(Axis::Z) * gizmoWorld);


        //Ray유효성 체크
        XMVECTOR dir = XMLoadFloat3(&mouseRay.direction);
        float dirLength = XMVectorGetX(XMVector3Length(dir));
        if (dirLength < 0.0001f)
        {
            hovered_ = Axis::Count;
            return;
        }
        hovered_ = Axis::Count;


        float minDist = FLT_MAX;
        auto updateAxis = [&](GizmoObject* obj, Axis axisType)
            {
                float dist = 0.0f;
                if (obj->intersects(mouseRay.origin, mouseRay.direction, dist))
                {
                    if (dist < minDist)
                    {
                        minDist = dist;
                        hovered_ = axisType;
                    }
                }
            };

        updateAxis(x_.get(), Axis::X);
        updateAxis(y_.get(), Axis::Y);
        updateAxis(z_.get(), Axis::Z);

        auto applyColor = [&](GizmoObject* obj, Axis axisType)
            {
                const Vec4& color = (hovered_ == axisType) ? AxisInfo::colorHover : AxisInfo::GetColor(axisType);
                obj->getMaterial()->setColor("MaterialColor", color);
            };
        applyColor(x_.get(), Axis::X);
        applyColor(y_.get(), Axis::Y);
        applyColor(z_.get(), Axis::Z);
    }

    void Gizmo::addToRenderQueue(Render::RenderCommandQueue* queue, const DirectX::XMMATRIX& viewMat)
    {
        if (!queue || currentShape_ == GizmoShape::Count || currentMode_ == TransformMode::Count) { return; }

        XMMATRIX invView = XMMatrixInverse(nullptr, viewMat);
        XMVECTOR camPos = invView.r[3];

        auto enqueue = [&](GizmoObject* obj)
            {
                XMMATRIX world = XMLoadFloat4x4(&obj->getWorldMatForShader());
                XMVECTOR diff = XMVectorSubtract(world.r[3], camPos);
                float depth = XMVectorGetX(XMVector3Length(diff));
                queue->addCommand(obj, depth);
            };
        if (x_) { enqueue(x_.get()); }
        if (y_) { enqueue(y_.get()); }
        if (z_) { enqueue(z_.get()); }
    }
}
