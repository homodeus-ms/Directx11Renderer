#include "FilterResources.hlsli"

float4 PS(VertexUVOutput input) : SV_TARGET
{
    float4 color = FilteredTexture.Sample(FilterSampler, input.uv);
    float l = (color.r + color.g + color.b) / 3.f;
    color = l > brightnessThreshold ? float4(color.xyz, 1.f) : float4(0.f, 0.f, 0.f, 1.f);
    
    return color;
}