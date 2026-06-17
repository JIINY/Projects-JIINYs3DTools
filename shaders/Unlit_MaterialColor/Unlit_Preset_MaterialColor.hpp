#pragma once
#include "render/Material.hpp"


namespace Render::Materials
{
    inline void setUnlitMaterialColorPreset(Material& mat)
    {
        mat.setVSPath(L"shaders/Unlit_MaterialColor/Unlit_VS_MaterialColor.hlsl");
        mat.setPSPath(L"shaders/Unlit_MaterialColor/Unlit_PS_MaterialColor.hlsl");

        mat.addProperty<DirectX::XMFLOAT4>("MaterialColor", DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
        mat.addProperty<float>("Roughness", 0.0f);
        mat.addProperty<float>("Metallic", 0.0f);
        mat.addProperty<DirectX::XMFLOAT2>("Padding", DirectX::XMFLOAT2(0, 0));

        mat.useProperty("MaterialColor", UI::WidgetData::Float4, UI::WidgetType::Float4Color, UI::Float4ColorData{Math::Vec4{1.0f, 1.0f, 1.0f, 1.0f}});
    }
}
