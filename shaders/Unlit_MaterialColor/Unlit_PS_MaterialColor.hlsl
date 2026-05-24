#include "../Common/Constants.hlsli"

struct PixelInput
{
    float4 position : SV_POSITION;
};

float4 psMain(PixelInput input) : SV_TARGET
{
    return MaterialColor;
}
