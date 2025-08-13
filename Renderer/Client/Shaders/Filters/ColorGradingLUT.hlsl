#include "FilterResources.hlsli"

float3 ApplyColorGrading(float3 color)
{
    float3 uvw = saturate(color);
    return LUTTexture.Sample(FilterSampler, uvw).rgb;
}

float4 PS(VertexUVOutput input) : SV_Target
{
    float3 baseColor = FilteredTexture.Sample(FilterSampler, input.uv).xyz;
    float3 graded = ApplyColorGrading(baseColor);
    float3 mixColor = baseColor * (1.f - strength) + graded * (strength);
    return float4(mixColor, 1.0f);
}