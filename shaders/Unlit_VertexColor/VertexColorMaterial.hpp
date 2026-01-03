#pragma once
#include "render/Material.hpp"


namespace Render 
{
    namespace Materials 
    {
        class VertexColorMaterial : public Material
        {
        public:
            VertexColorMaterial() {}
            virtual ~VertexColorMaterial() {}

            bool initialize(ID3D11Device* device, const std::wstring& vsPath, const std::wstring& psPath) 
            {
                Material::initialize(device);

                loadVertexShader(device, vsPath, "vsMain");
                loadPixelShader(device, psPath, "psMain");

                return true;
            }
        };
    }
}
