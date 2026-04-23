#pragma once
#include "common/Math.hpp"
#include "../src/render/Material.hpp"


namespace Render::Materials
{
    class BlinnPhongMaterial : public Material
    {
    public:
        virtual ~BlinnPhongMaterial() = default;

        void initialize(ID3D11Device* device) override;
        void bind(ID3D11DeviceContext* context) override;

        void setBaseColor(float r, float g, float b, float a = 1.0f) { setColor("Color", Math::Vec4(r, g, b, a)); }
        void setRoughness(float r) { setProperty("Roughness", r); }
    };
}
