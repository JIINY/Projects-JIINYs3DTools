#pragma once
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>
#include "common/Math.hpp"
#include "common/Mode.hpp"
#include "object/Entity.hpp"
#include "render/tools/GizmoObject.hpp"

class MaterialManager;
namespace Render { class RenderCommandQueue; }


namespace Render::Tools
{
    class Gizmo : public Objects::Entity
    {
    public:
        Gizmo();
        virtual ~Gizmo();

        bool initialize(ID3D11Device* device, MaterialManager* matManager);
        void update(const Math::Ray& mouseRay, const DirectX::XMMATRIX& targetWorld);

        void addToRenderQueue(Render::RenderCommandQueue* queue, const DirectX::XMMATRIX& viewMat);
        void setMode(TransformMode mode);
        TransformMode getMode() const { return currentMode_; }
        Math::Axis getHoveredAxis() const { return hovered_; }

        GizmoObject* getX() const { return x_.get(); }
        GizmoObject* getY() const { return y_.get(); }
        GizmoObject* getZ() const { return z_.get(); }


    private:
        std::unique_ptr<Render::Tools::GizmoObject> x_;
        std::unique_ptr<Render::Tools::GizmoObject> y_;
        std::unique_ptr<Render::Tools::GizmoObject> z_;

        TransformMode currentMode_ = TransformMode::Count;
        GizmoShape currentShape_ = GizmoShape::Count;
        Math::Axis hovered_ = Math::Axis::Count;

        ID3D11Device* device_ = nullptr;
        MaterialManager* materialManager_ = nullptr;
    };
}
