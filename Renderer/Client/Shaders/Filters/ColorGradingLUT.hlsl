#include "FilterResources.hlsli"

float3 ApplyColorGrading(float3 color)
{
    float3 uvw = saturate(color);
    return LUTTexture.Sample(FilterSampler, uvw).rgb;
}

float4 PS(VertexUVOutput input) : SV_Target
{
    float3 baseColor = FilterTargetTexture.Sample(FilterSampler, input.uv).xyz;
    float3 graded = ApplyColorGrading(baseColor);
    return float4(graded, 1.0f);
}