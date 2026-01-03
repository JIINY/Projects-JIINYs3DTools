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


struct VS_OUT
{
    float4 pos : SV_Position;
};

VS_OUT vsMain(float4 pos : POSITION)
{
    VS_OUT output;
    output.pos = mul(pos, World);
    output.pos = mul(output.pos, View);
    output.pos = mul(output.pos, Projection);
    return output;
}
