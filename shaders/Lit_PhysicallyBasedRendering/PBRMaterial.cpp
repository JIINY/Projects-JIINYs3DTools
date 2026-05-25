#include "PBRMaterial.hpp"
using namespace std;
using namespace DirectX;


namespace Render::Materials
{
    void PBRMaterial::initialize(ID3D11Device* device)
    {
        Material::initialize(device);

        vsPath_ = L"shaders/Lit_BlinnPhong/Lit_VS_BlinnPhong.hlsl";
        psPath_ = L"shaders/Lit_PhysicallyBasedRendering/Lit_PS_PBR.hlsl";

        addProperty<XMFLOAT4>("MaterialColor", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
        addProperty<float>("Roughness", 0.0f);
        addProperty<float>("Metallic", 0.0f);
        addProperty<XMFLOAT2>("Padding", XMFLOAT2(0, 0));

        createBuffer(device);
    }
}
