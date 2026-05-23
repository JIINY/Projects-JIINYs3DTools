cbuffer MaterialData : register(b1)
{
    float4 MaterialColor;
}

struct VertexOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 psMain(VertexOutput input) : SV_TARGET
{
    return MaterialColor;
}
