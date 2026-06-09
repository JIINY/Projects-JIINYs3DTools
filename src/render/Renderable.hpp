#pragma once
#include <d3d11.h>
#include <cstdint>
#include "common/Math.hpp"
#include "IRenderable.hpp"
#include "RenderCommand.hpp"


namespace Render
{
    class Renderable : public IRenderable
    {
    public:
        virtual ~Renderable() = default;

        virtual void draw(ID3D11DeviceContext* context, const Math::Mat4& view, const Math::Mat4& proj, std::shared_ptr<PixelShader> overridePS = nullptr) = 0;

        void setVisible(bool visible) { isVisible_ = visible; }
        bool isVisible() const { return isVisible_; };

        void setCastShadow(bool enable) { castShadow_ = enable; }
        bool isCastShadow() const { return castShadow_; }
        void setReceiveShadow(bool enable) { receiveShadow_ = enable; }
        bool isReceiveShadow() const { return receiveShadow_; }

        void setRenderQueue(int queue) { renderQueue_ = queue; }
        virtual int getRenderQueue() const override { return renderQueue_; }
        void setLayer(uint32_t layer) { layer_ = layer; }
        virtual uint32_t getLayer() const override { return layer_; }
        void setCullable(bool enable) { isCullable_ = enable; }
        bool isCullable() const { return isCullable_; }
        void setDynamic(bool enable) { isDynamic_ = enable; }
        bool isDynamic() const { return isDynamic_; }


    protected:
        bool isVisible_ = true;
        bool isDynamic_ = false;
        bool castShadow_ = true;
        bool receiveShadow_ = true;
        bool isCullable_ = true;

        int renderQueue_ = static_cast<int>(RenderQueue::Geometry); //TO_DO: 오브젝트에서 저장할지, Elements에서 저장할지, Material에서 저장할지 결정 후 Save에도 반영
        uint32_t layer_ = 1;
    };
}
