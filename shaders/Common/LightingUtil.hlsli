#ifndef __LIGHTING_UTIL_HLSLI__
#define __LIGHTING_UTIL_HLSLI__

#include "Constants.hlsli"

void ComputeLightValues(float3 posW, out float3 lightVec, out float attenuation)
{
    lightVec = SunDirection;
    attenuation = 1.0f;
}

//Blinn-Phong (Realistic)
float3 CalcBlinnPhong(float3 normal, float3 toEye, float3 posW)
{
    float3 toLight;
    float attenuation;
    ComputeLightValues(posW, toLight, attenuation);

    float ndotl = max(dot(normal, toLight), 0.0f);
    float3 lightStrength = SunColor * SunIntensity;
    
    //Diffuse 난반사
    float3 diffuse = ndotl * lightStrength * MaterialColor.rgb;
    
    //Specular 정반사
    float3 halfVec = normalize(toLight + toEye);
    float ndothVec = max(dot(normal, halfVec), 0.0f);
    float specPower = (1.0f - Roughness) * 255.0f + 1.0f; //Roughness를 Specular로 변환. 1.0 거침(퍼지는 빛) <> 0.0 매끄러움(쨍한 빛)
    float3 specular = pow(ndothVec, specPower) * lightStrength;
    
    return (diffuse + specular) * attenuation;
}


//half-Lambert
float3 CalcHalfLambert(float3 normal, float3 posW)
{
    float3 toLight;
    float attenuation;
    ComputeLightValues(posW, toLight, attenuation);
    
    //공식 (dot * 0.5) + 0.5
    float ndotl = dot(normal, toLight);
    float halfLambert = ndotl * 0.5f + 0.5f;
    halfLambert = halfLambert * halfLambert; //제곱해서 대비감을 줌
    
    float3 diffuse = halfLambert * SunColor * SunIntensity;
    
    return diffuse * attenuation * MaterialColor.rgb;
}


//PBR
//D: 법선 분포 함수(Normal Distribution) 미세면 중 halfVec방향을 향한 것의 비율
//roughness가 작을 수록 한 점에 몰려 하이라이트가 쨍해짐
float DistributionGGX(float nDotH, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float denom = nDotH * nDotH * (a2 - 1.0f) + 1.0f;
    
    denom = PI * denom * denom;
    
    return a2 / max(denom, 0.0001f); //devide0 방지
}

//G: 기하 감쇠 함수(Geometry) 미세면끼리 서로 그림자를 드리우거나 빛을 가리는 정도
//거칠수록 더 많이 가려져 어두워짐
float GeometrySchlickGGX(float nDotX, float roughness)
{
    float r = roughness + 1.0f;
    float k = (r * r) / 8.0f;
    
    return nDotX / (nDotX * (1.0f - k) + k);
}

float GeometrySmith(float nDotV, float nDotL, float roughness)
{
    //빛 쪽 가림 * 시선 쪽 가림
    return GeometrySchlickGGX(nDotV, roughness) * GeometrySchlickGGX(nDotL, roughness);
}

//F: 프레넬(Fresnel) 보는 각도가 비스듬할수록 반사가 강해지는 현상
//F0는 정면에서의 반사율, hDotV가 작을수록(=비스듬할 수록) 1에 가까워짐
float3 FresnelSchlick(float hDotV, float3 F0)
{
    return F0 + (1.0f - F0) * pow(1.0f - hDotV, 5.0f);
}

float3 CalcPBR(float3 normal, float3 toEye, float3 albedo, float metallic, float roughness, float3 toLight, float3 radiance)
{
    //분포D, 기하G, 프레넬F를 계산하기 위한 공통 벡터들 준비
    float3 halfVec = normalize(toLight + toEye);
    float nDotL = max(dot(normal, toLight), 0.0f);
    float nDotV = max(dot(normal, toEye), 0.0f);
    float nDotH = max(dot(normal, halfVec), 0.0f);
    float hDotV = max(dot(halfVec, toEye), 0.0f);
    
    //F0: 표면의 기본 반사율(정면에서 볼 때 표면이 반사하는 비율). 
    //비금속(dielectric)은 0.04로 거의 고정, 금속은 albedo 자체가 반사색으로 metalic으로 그 사이를 보간
    float F0 = lerp(float3(0.04f, 0.04f, 0.04f), albedo, metallic);
    
    
    //Specular: D * G * F / (4 * nDotV * nDotL)
    float D = DistributionGGX(nDotH, roughness);
    float G = GeometrySmith(nDotV, nDotL, roughness);
    float3 F = FresnelSchlick(hDotV, F0);
    
    float3 specular = (D * G * F) / max(4.0f * nDotV * nDotL, 0.0001f);
    
    //Diffuse
    //F는 반사된 빛의 비율 -> 나머지(1-F)가 난반사로 감, 금속은 난반사가 없으므로 metallic만큼 추가로 깎음
    float3 kD = (float3(1.0f, 1.0f, 1.0f) - F) * (1.0f - metallic);
    float3 diffuse = kD * albedo / PI;
    
    //radiance = 광원색 * 강도 * 감쇠 (호출하는 쪽에서 미리 곱해서 넘김)
    return (diffuse + specular) * radiance * nDotL;
}
#endif
