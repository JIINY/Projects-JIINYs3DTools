#include "Gizmo.hpp"
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>
#include <algorithm>
#include "common/Math.hpp"
#include "common/Mode.hpp"
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

    bool Gizmo::initialize(ID3D11Device* device)
    {
        device_ = device;
        red_ = make_shared<PixelShader>();
        green_ = make_shared<PixelShader>();
        blue_ = make_shared<PixelShader>();
        yellow_ = make_shared<PixelShader>();

        if (!red_->initialize(device_, L"resources/Unlit_Common/GizmoX_PS_Red.hlsl", "psMain")) { red_ = nullptr; }
        if (!green_->initialize(device_, L"resources/Unlit_Common/GizmoY_PS_Green.hlsl", "psMain")) { green_ = nullptr; }
        if (!blue_->initialize(device_, L"resources/Unlit_Common/GizmoZ_PS_Blue.hlsl", "psMain")) { blue_ = nullptr; }
        if (!yellow_->initialize(device, L"resources/Unlit_Common/GizmoHovered_PS_Yellow.hlsl", "psMain")) { yellow_ = nullptr; }

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

        GizmoData data = getData(x_, currentShape_);
        x_->initialize(device_, data, Axis::X);
        y_->initialize(device_, data, Axis::Y);
        z_->initialize(device_, data, Axis::Z);
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
    }

    void Gizmo::addToRenderQueue(Render::RenderCommandQueue* queue, const DirectX::XMMATRIX& viewMat)
    {
        if (!queue || currentShape_ == GizmoShape::Count || currentMode_ == TransformMode::Count) { return; }

        XMMATRIX invView = XMMatrixInverse(nullptr, viewMat);
        XMVECTOR camPos = invView.r[3];

        if (x_)
        {
            XMMATRIX world = XMLoadFloat4x4(&x_->getWorldMatForShader());
            XMVECTOR oPos = world.r[3];
            XMVECTOR diff = XMVectorSubtract(oPos, camPos);
            float depth = XMVectorGetX(XMVector3Length(diff));

            shared_ptr<PixelShader> ps = (hovered_ == Axis::X) ? yellow_ : red_;
            queue->addCommand(x_.get(), depth, ps);
        }
        if (y_)
        {
            XMMATRIX world = XMLoadFloat4x4(&y_->getWorldMatForShader());
            XMVECTOR oPos = world.r[3];
            XMVECTOR diff = XMVectorSubtract(oPos, camPos);
            float depth = XMVectorGetX(XMVector3Length(diff));

            shared_ptr<PixelShader> ps = (hovered_ == Axis::Y) ? yellow_ : green_;
            queue->addCommand(y_.get(), depth, ps);
        }
        if (z_)
        {
            XMMATRIX world = XMLoadFloat4x4(&z_->getWorldMatForShader());
            XMVECTOR oPos = world.r[3];
            XMVECTOR diff = XMVectorSubtract(oPos, camPos);
            float depth = XMVectorGetX(XMVector3Length(diff));

            shared_ptr<PixelShader> ps = (hovered_ == Axis::Z) ? yellow_ : blue_;
            queue->addCommand(z_.get(), depth, ps);
        }
    }
}
