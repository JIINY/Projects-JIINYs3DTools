#pragma once
#include "render/Material.hpp"


namespace Render::Materials
{
    inline void setUnlitVertexColorPreset(Material& mat)
    {
        mat.setVSPath(L"shaders/Unlit_VertexColor/Unlit_VS_VertexColor.hlsl");
        mat.setPSPath(L"shaders/Unlit_VertexColor/Unlit_PS_VertexColor.hlsl");
    }
}
