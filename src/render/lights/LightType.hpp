#pragma once
#include <DirectXMath.h>


namespace Render 
{
    enum class LightType : int
    {
        Directional = 0,
        Point = 1,
        Spot = 2,
        Count = 3
    };

    struct LightData //HLSL ConstantBuffer와 100% 일치해야하는 구조체(16바이트 규칙)
    {
        DirectX::XMFLOAT3 pos = { 0, 0, 0 };
        float range = 10.0f;

        DirectX::XMFLOAT3 direction = { 0, 1, 0 };
        float intensity = 1.0f;

        DirectX::XMFLOAT3 color = { 1, 1, 1 };

        float spotAngle = 45.0f;
        int type = 0; //LightType
        DirectX::XMFLOAT3 padding = { 0, 0, 0 };
    };
}
