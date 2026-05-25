#ifndef __CONSTANTS_HLSLI__
#define __CONSTANTS_HLSLI__

#define MAX_LIGHTS 16
#define PI 3.14159265f

#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2


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

struct LightData
{
    float3 pos; //row0
    float range;
    
    float3 direction; //row1
    float intensity;
    
    float3 color; //row2
    float spotAngle;
    
    int type; //row3
    float3 padding;
};

cbuffer LightBuffer : register(b3)
{
    LightData Lights[MAX_LIGHTS];
    int LightCount;
    float3 lightBufferPadding;
};

#endif
