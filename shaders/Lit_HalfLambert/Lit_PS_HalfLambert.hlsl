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
    input.normal = normalize(input.normal);
    
    float3 color = CalcHalfLambert(input.normal, input.posW);
    color += float3(0.1f, 0.1f, 0.1f) * MaterialColor.rgb; //Ambient
    
    return float4(color, 1.0f);
}
