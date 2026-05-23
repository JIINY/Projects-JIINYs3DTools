#pragma once
#include "render/Material.hpp"


namespace Render::Materials
{
    class VertexColorMaterial : public Material
    {
    public:
        virtual ~VertexColorMaterial() = default;

        void initialize(ID3D11Device* device) 
        {
            Material::initialize(device);

            vsPath_ = L"shaders/Unlit_VertexColor/Unlit_VS_VertexColor.hlsl";
            psPath_ = L"shaders/Unlit_VertexColor/Unlit_PS_VertexColor.hlsl";
        }
    };
}
