#pragma once
#include "common/Math.hpp"
#include "../src/render/Material.hpp"


namespace Render::Materials
{
    class PBRMaterial : public Material
    {
    public:
        virtual ~PBRMaterial() = default;

        void initialize(ID3D11Device* device) override;

        void setBaseColor(float r, float g, float b, float a = 1.0f) { setColor("MaterialColor", Math::Vec4(r, g, b, a)); }
        void setRoughness(float r) { setProperty("Roughness", r); }
        void setMetallic(float m) { setProperty("Metallic", m); }
    };
}
