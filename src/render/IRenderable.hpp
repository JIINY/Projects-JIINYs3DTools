#pragma once
#include <d3d11.h>
#include <cstdint>
#include "common/Math.hpp"


namespace Render 
{
    class PixelShader;

    class IRenderable 
    {
    public:
        virtual ~IRenderable() = default;

        virtual void draw(ID3D11DeviceContext* context, const Math::Mat4& view, const Math::Mat4& proj, std::shared_ptr<PixelShader> overridePS = nullptr) = 0;

        virtual int getRenderQueue() const = 0;
        virtual uint32_t getLayer() const = 0;
    };
}
