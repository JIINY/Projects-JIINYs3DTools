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
#endif
