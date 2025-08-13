#include "FilterResources.hlsli"

float4 PS(VertexUVOutput input) : SV_Target
{
    float3 color = float3(0.f, 0.f, 0.f);
    
    int i;
    for (i = 0; i < 5; i++)
    {
        color += GAUSSIAN_WEIGHTS[i] * FilteredTexture.Sample(FilterSampler, input.uv + float2(0.f, dy) * float(i - 2)).rgb;
    }
    
    return float4(color, 1.f);
}