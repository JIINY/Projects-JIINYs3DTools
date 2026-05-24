#include "MaterialColorMaterial.hpp"
#include <DirectXMath.h>
#include "common/Math.hpp"
using namespace std;
using namespace Math;
using namespace DirectX;


namespace Render::Materials
{
    void MaterialColorMaterial::initialize(ID3D11Device* device) 
    {
        Material::initialize(device);

        vsPath_ = L"shaders/Unlit_MaterialColor/Unlit_VS_MaterialColor.hlsl";
        psPath_ = L"shaders/Unlit_MaterialColor/Unlit_PS_MaterialColor.hlsl";

        addProperty<XMFLOAT4>("MaterialColor", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
        addProperty<float>("Roughness", 0.0f);
        addProperty<XMFLOAT3>("Padding", XMFLOAT3(0, 0, 0));

        createBuffer(device);
    }

    void MaterialColorMaterial::setBaseColor(const Vec4& color)
    {
        setColor("MaterialColor", color);
    }
}
