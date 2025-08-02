#include "FilterResources.hlsli"

float4 PS(VertexUVOutput input) : SV_Target
{
    float4 color = FilterTargetTexture.Sample(FilterSampler, input.uv);
    return float4(color.xyz, 1.f);
}

