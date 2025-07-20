#ifndef GET_COLOR_FUNCS_HLSLI
#define GET_COLOR_FUNCS_HLSLI

#include "Defines.hlsli"
#include "Global.hlsli"
#include "Light.hlsli"

float4 GetToonShadingByGlobalLight(float4 litColor, float3 normal, float2 uv)
{
    float3 diffuse = BLACK;
    
    float lDotN = saturate(dot(-GlobalLight.direction, normal));
    float albedo = DiffuseMap.Sample(LinearSampler, uv);
    diffuse = albedo * GlobalLight.diffuse.xyz * Material.diffuse.xyz * lDotN;
    
    diffuse = saturate(diffuse);
    diffuse = ceil(diffuse * 3) / 3.f;
    
    return float4(litColor.xyz * diffuse.xyz * 1.5, 1.f);
}



#endif /* GET_COLOR_FUNCS_HLSLI */