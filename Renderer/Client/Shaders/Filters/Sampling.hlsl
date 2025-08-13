#include "FilterResources.hlsli"

float4 PS(VertexUVOutput input) : SV_Target
{
    float4 color = FilteredTexture.Sample(FilterSampler, input.uv);
    return float4(color.xyz, 1.f);
}

