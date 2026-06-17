#pragma once
#include "common/Math.hpp"
#include "render/Material.hpp"
#include "ui/WidgetData.hpp"


namespace Render::Materials
{
    inline void setLitBlinnPhongPreset(Material& mat)
    {
        mat.setVSPath(L"shaders/Lit_BlinnPhong/Lit_VS_BlinnPhong.hlsl");
        mat.setPSPath(L"shaders/Lit_BlinnPhong/Lit_PS_BlinnPhong.hlsl");

        mat.addProperty<DirectX::XMFLOAT4>("MaterialColor", DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
        mat.addProperty<float>("Roughness", 0.9f);  //Display를 사용하는 경우 초기값과 ResetValue맞춰야 함
        mat.addProperty<float>("Metallic", 0.0f);
        mat.addProperty<DirectX::XMFLOAT2>("Padding", DirectX::XMFLOAT2(0, 0));

        mat.useProperty("MaterialColor", UI::WidgetData::Float4, UI::WidgetType::Float4Color, 
            UI::Float4ColorData{Math::Vec4{1.0f, 1.0f, 1.0f, 1.0f}});
        mat.useProperty("Roughness", UI::WidgetData::Float, UI::WidgetType::FloatDragnSlider,
            UI::FloatDragnSliderData{ 0.9f, 0.2f, 0.9f, 1.0f, 0.01f, "%.2f", true, UI::DisplayFloatData{0.0f, 1.0f, 0.1f, "%.1f"}});
    }
}
