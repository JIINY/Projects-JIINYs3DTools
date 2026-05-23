#include "../Common/Constants.hlsli"
#include "../Common/LightingUtil.hlsli"

//정점 입력(Vertex Input)
struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

//정점 출력(Vertex Output)
struct VertexOutput
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

VertexOutput vsMain(VertexInput input)
{
    VertexOutput output;

    //월드 위치 계산(Model -> World)
    float4 worldPos = mul(float4(input.position, 1.0f), World);
    output.posW = worldPos.xyz;
	
    //화면 위치 계산(World->View->Projection)
    float4 viewPos = mul(worldPos, View);
    output.posH = mul(viewPos, Projection);
    
    //법선(Normal) 변환(Model -> World)
    output.normal = mul(input.normal, (float3x3) World);
    
    //UV 전달
    output.uv = input.uv;

    return output;
}
