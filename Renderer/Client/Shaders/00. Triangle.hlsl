#include "struct.hlsli"

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    
    output.position = mul(input.position, World);
    //output.position = mul(output.position, matView);
    //output.position = mul(output.position, matProjection);
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) World);
    return output;
}

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 PS(VertexOutput input) : SV_Target
{
    float4 color = texture0.Sample(sampler0, input.uv);
    
    float4 temp = { 1.f, 0.f, 0.f, 1.f };
    return temp;
}