#include "../Common/Constants.hlsli"

//정점 입력(Vertex Input)
struct VertexInput
{
    float3 position : POSITION;
    float4 color : COLOR;
};

//정점 출력(Vertex Output)
struct VertexOutput
{
    float4 position : SV_POSITION; // 레스터라이즈한 2D 화면 좌표
    float4 color : COLOR; //픽셀 셰이더로 전달할 색상
};

VertexOutput vsMain(VertexInput input)
{
    VertexOutput output;

    float4 worldPos = mul(float4(input.position, 1.0f), World);
    float4 viewPos = mul(worldPos, View);
    output.position = mul(viewPos, Projection);
    output.color = input.color;

    return output;
}
