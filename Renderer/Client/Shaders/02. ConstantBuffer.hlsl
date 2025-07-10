#include "struct.hlsli"

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    
    output.position = mul(input.position, World);
    output.position = mul(output.position, VP);
   
    output.uv = input.uv;
    return output;
}

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 PS(VertexOutput input) : SV_Target
{
    float4 SampledColor = texture0.Sample(sampler0, input.uv);
    float4 color = SampledColor * LightDiffuse;
	
    return color;

}