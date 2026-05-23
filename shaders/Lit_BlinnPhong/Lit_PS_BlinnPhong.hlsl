#include "../Common/Constants.hlsli"
#include "../Common/LightingUtil.hlsli"

struct PixelInput
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

float4 psMain(PixelInput input) : SV_TARGET
{
    float3 normal = normalize(input.normal); //정규화 필수
    float3 toEye = normalize(CameraPos - input.posW);

    float3 color = CalcBlinnPhong(normal, toEye, input.posW);

    float upY = input.normal.y;
    float3 ambient = float3(0.0f, 0.0f, 0.0f);
    
    if (upY > 0.0f) { ambient = lerp(AmbientMiddle, AmbientTop, upY); }
    else { ambient = lerp(AmbientMiddle, AmbientBottom, -upY); }

    color += ambient * MaterialColor.rgb;
    
    return float4(color, 1.0f);
}
