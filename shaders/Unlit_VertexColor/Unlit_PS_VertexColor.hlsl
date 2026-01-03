struct VertexOutput
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

float4 psMain(VertexOutput input) : SV_Target
{
    return input.color;
}
