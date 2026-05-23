#include "HalfLambertMaterial.hpp"
using namespace std;
using namespace DirectX;


namespace Render::Materials
{
    void HalfLambertMaterial::initialize(ID3D11Device* device)
    {
        Material::initialize(device);

        vsPath_ = L"shaders/Lit_HalfLambert/Lit_VS_BlinnPhong.hlsl";
        psPath_ = L"shaders/Lit_HalfLambert/Lit_PS_HalfLambert.hlsl";

        addProperty<XMFLOAT4>("MaterialColor", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
        addProperty<float>("Roughness", 0.0f);
        addProperty<XMFLOAT3>("Padding", XMFLOAT3(0, 0, 0));

        createBuffer(device);
    }
}
