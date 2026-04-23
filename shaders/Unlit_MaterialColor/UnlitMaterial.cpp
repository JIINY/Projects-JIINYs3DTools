#include "UnlitMaterial.hpp"
#include <DirectXMath.h>
#include "common/Math.hpp"
using namespace std;
using namespace Math;
using namespace DirectX;


namespace Render::Materials
{
    bool UnlitMaterial::initialize(ID3D11Device* device, const wstring& vsPath, const wstring& psPath) 
    {
        Material::initialize(device);

        loadVertexShader(device, vsPath, "vsMain");
        loadPixelShader(device, psPath, "psMain");

        addProperty<XMFLOAT4>("Color", XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
        createBuffer(device);
        return true;
    }

    void UnlitMaterial::setBaseColor(const Vec4& color) 
    {
        setColor("Color", color);
    }
}
