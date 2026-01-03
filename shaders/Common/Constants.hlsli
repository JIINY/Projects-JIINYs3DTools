#ifndef __CONSTANTS_HLSLI__
#define __CONSTANTS_HLSLI__

cbuffer SceneConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
    float3 CameraPos;
    float padding1;
    
    float3 SunDirection;
    float SunIntensity;
    float3 SunColor;
    float padding2;
    
    float3 AmbientTop;
    float padding3;
    float3 AmbientMiddle;
    float padding4;
    float3 AmbientBottom;
    float padding5;
}

cbuffer WorldMatrixBuffer : register(b1)
{
    matrix World;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 MaterialColor;
    float Roughness;
    float3 MatPad;
};

#endif
