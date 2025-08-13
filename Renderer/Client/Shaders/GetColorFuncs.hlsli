#ifndef GET_COLOR_FUNCS_HLSLI
#define GET_COLOR_FUNCS_HLSLI

#include "Defines.hlsli"
#include "Global.hlsli"
#include "Light.hlsli"

float4 GetToonShadingByGlobalLight(float3 litColor, float3 normal, float2 uv)
{
    float3 diffuse = BLACK.xyz;
    
    float lDotN = saturate(dot(-GlobalLight.direction, normal));
    float4 sampled = AlbedoMap.Sample(LinearSampler, uv);
    diffuse = sampled.xyz * GlobalLight.diffuse.xyz * Material.diffuse.xyz * lDotN;
    
    diffuse = saturate(diffuse);
    diffuse = ceil(diffuse * 3) / 3.f;
    
    return float4(litColor * diffuse * 1.5f, 1.f);
}



#endif /* GET_COLOR_FUNCS_HLSLI */