#pragma once
#include "render/Material.hpp"


namespace Render::Materials
{
    inline void setLitHalfLambertPreset(Material& mat)
    {
        mat.setVSPath(L"shaders/Lit_BlinnPhong/Lit_VS_BlinnPhong.hlsl");
        mat.setPSPath(L"shaders/Lit_HalfLambert/Lit_PS_HalfLambert.hlsl");

        mat.addProperty<DirectX::XMFLOAT4>("MaterialColor", DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
        mat.addProperty<float>("Roughness", 0.0f);
        mat.addProperty<float>("Metallic", 0.0f);
        mat.addProperty<DirectX::XMFLOAT2>("Padding", DirectX::XMFLOAT2(0, 0));

        mat.useProperty("MaterialColor", UI::WidgetData::Float4, UI::WidgetType::Float4Color, UI::Float4ColorData{Math::Vec4{1.0f, 1.0f, 1.0f, 1.0f}});
    }
}
