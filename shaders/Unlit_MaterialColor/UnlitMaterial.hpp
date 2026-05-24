#pragma once
#include "render/Material.hpp"
#include "common/Math.hpp"


namespace Render::Materials
{
    class UnlitMaterial : public Material
    {
    public:
        virtual ~UnlitMaterial() = default;

        void initialize(ID3D11Device* device);

        void setBaseColor(const Math::Vec4& color);
    };
}
