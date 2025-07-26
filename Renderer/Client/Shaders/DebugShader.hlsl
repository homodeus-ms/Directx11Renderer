#include "Global.hlsli"

VertexUVOutput VS(VertexUVInput input)
{
    VertexUVOutput output;
    float4 worldPos = mul(float4(input.position.xyz, 1.f), W);
    output.position = mul(worldPos, VP);
    output.uv = input.uv;
    
    return output;
}

float4 PS(VertexUVOutput input) : SV_Target
{
    //float sampled = ShadowMaps[0].Sample(LinearSampler, input.uv).r;
    //return float4(sampled.rrr, 1.f);
    return BLACK;
}