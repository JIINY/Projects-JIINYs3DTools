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
    float ScenePadding1;
    
    float3 SunDirection;
    float SunIntensity;
    float3 SunColor;
    float ScenePadding2;
    
    float3 AmbientTop;
    float ScenePadding3;
    float3 AmbientMiddle;
    float ScenePadding4;
    float3 AmbientBottom;
    float ScenePadding5;
}

cbuffer WorldMatrixBuffer : register(b1)
{
    matrix World;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 MaterialColor;
    float Roughness;
    float Metallic;
    float2 MatPadding;
};

struct LightData
{
    float3 Pos; //row0
    float Range;
    
    float3 Direction; //row1
    float Intensity;
    
    float3 Color; //row2
    float SpotAngle;
    
    int LightType; //row3
    float3 Padding;
};

cbuffer LightBuffer : register(b3)
{
    LightData Lights[MAX_LIGHTS];
    int LightCount;
    float3 LightBufferPadding;
};

#endif
