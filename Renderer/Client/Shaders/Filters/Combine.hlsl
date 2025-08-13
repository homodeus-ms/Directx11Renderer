#include "FilterResources.hlsli"

float4 PS(VertexUVOutput input) : SV_Target
{
    float3 filtered = FilteredTexture.Sample(FilterSampler, input.uv).xyz;
    float3 originalColor = OriginalTexture.Sample(FilterSampler, input.uv).xyz;
    
    return float4(originalColor * (1.f - strength) + filtered * strength, 1.f);
}