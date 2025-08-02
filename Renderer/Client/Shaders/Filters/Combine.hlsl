#include "FilterResources.hlsli"

float4 PS(VertexUVOutput input) : SV_Target
{
    float3 filtered = FilterTargetTexture.Sample(FilterSampler, input.uv).xyz;
    float3 originalColor = OriginalTexture.Sample(FilterSampler, input.uv).xyz;
    
    return float4(originalColor + filtered * strength, 1.f);
}