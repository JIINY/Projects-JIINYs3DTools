#include "../Common/Constants.hlsli"
#include "../Common/LightingUtil.hlsli"

struct PixelInput
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

float4 psMain(PixelInput input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    float3 toEye = normalize(CameraPos - input.posW);
    
    float3 albedo = MaterialColor.rgb;
    float roughness = Roughness;
    float metallic = 0.0f;

    float3 result = float3(0.0f, 0.0f, 0.0f);
    
    //태양광 계산
    {
        float3 toLight = normalize(SunDirection);
        float3 radiance = SunColor * SunIntensity;

        result += CalcPBR(normal, toEye, albedo, metallic, roughness, toLight, radiance);
    }
    
    //다른 광원 계산
    for (int i = 0; i < LightCount; ++i)
    {
        LightData light = Lights[i];
        
        float3 toLight;
        float attenuation; //감쇠
        
        if (light.type == LIGHT_DIRECTIONAL)
        {
            toLight = normalize(light.direction);
            attenuation = 1.0f;
        }
        else //거리 감쇠
        {
            float3 toLightV = light.pos - input.posW;
            float dist = length(toLightV);
            toLight = toLightV / max(dist, 0.0001f);
            
            float d = saturate(1.0f - (dist / max(light.range, 0.0001f)));
            attenuation = d * d;

            if (light.type == LIGHT_SPOT) //추가로 원뿔 감쇠
            {
                float cosAngle = dot(normalize(light.direction), -toLight);
                float cosCutoff = cos(radians(light.spotAngle));
                float coneAttenuation = saturate((cosAngle - cosCutoff) / (1.0f - cosCutoff));
                
                attenuation *= coneAttenuation;
            }
        }
        
        float3 radiance = light.color * light.intensity * attenuation;
        result += CalcPBR(normal, toEye, albedo, metallic, roughness, toLight, radiance);
    }

    //Ambient
    float upY = normal.y;
    float3 ambient;
    if (upY > 0.0f)
    {
        ambient = lerp(AmbientMiddle, AmbientTop, upY);
    }
    else
    {
        ambient = lerp(AmbientMiddle, AmbientBottom, -upY);
    }
    
    result += (ambient * albedo);
    return float4(result, 1.0f);
};
