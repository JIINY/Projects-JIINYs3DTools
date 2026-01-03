//상수 버퍼(Constant Buffer)
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


//정점 입력(Vertex Input)
struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
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
