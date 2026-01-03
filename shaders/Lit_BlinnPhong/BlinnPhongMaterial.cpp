#include "BlinnPhongMaterial.hpp"


namespace Render 
{
    namespace Materials 
    {
        void BlinnPhongMaterial::initialize(ID3D11Device* device)
        {
            Material::initialize(device);

            loadVertexShader(device, L"shaders/Lit_BlinnPhong/Lit_VS_BlinnPhong.hlsl");
            loadPixelShader(device, L"shaders/Lit_BlinnPhong/Lit_PS_BlinnPhong.hlsl");

            createBuffer<MaterialData>(device);
        }

        void BlinnPhongMaterial::bind(ID3D11DeviceContext* context)
        {
            Material::bind(context);
            updateData(context, data_);

            if (constantBuffer_) { constantBuffer_->bindPS(context, 2); }
        }

    }
}