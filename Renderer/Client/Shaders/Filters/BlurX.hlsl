#include "FilterResources.hlsli"

float4 PS(VertexUVOutput input) : SV_Target
{
    float3 color = float3(0.f, 0.f, 0.f);
    
    int i;
    float2 uv = input.uv;
    
    for (i = 0; i < 5; i++)
    {
        float2 offset = float2(dx * (i - 2), 0.0f);
        color += GAUSSIAN_WEIGHTS[i] * FilterTargetTexture.Sample(FilterSampler, uv + offset).rgb;
    }
    
    return float4(color, 1.f);
}