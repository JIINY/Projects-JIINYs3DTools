#pragma once
#include "render/Material.hpp"


namespace Render::Materials
{
    inline void setLitPBRPreset(Material& mat)
    {
        mat.setVSPath(L"shaders/Lit_BlinnPhong/Lit_VS_BlinnPhong.hlsl");
        mat.setPSPath(L"shaders/Lit_PhysicallyBasedRendering/Lit_PS_PBR.hlsl");

        mat.addProperty<DirectX::XMFLOAT4>("MaterialColor", DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
        mat.addProperty<float>("Roughness", 0.0f);
        mat.addProperty<float>("Metallic", 0.0f);
        mat.addProperty<DirectX::XMFLOAT2>("Padding", DirectX::XMFLOAT2(0, 0));

        mat.useProperty("MaterialColor", UI::WidgetData::Float4, UI::WidgetType::Float4Color,
            UI::Float4ColorData{Math::Vec4{1.0f, 1.0f, 1.0f, 1.0f}});
        mat.useProperty("Roughness", UI::WidgetData::Float, UI::WidgetType::FloatDragnSlider,
            UI::FloatDragnSliderData{ 0.0f, 0.2f, 0.0f, 1.0f, 0.1f});
        mat.useProperty("Metallic", UI::WidgetData::Float, UI::WidgetType::FloatDragnSlider,
            UI::FloatDragnSliderData{ 0.0f, 0.2f, 0.0f, 1.0f, 0.1f});
    }
}
