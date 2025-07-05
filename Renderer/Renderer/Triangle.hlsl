
struct VS_Input
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

cbuffer TransformData : register(b0)
{
    float4 offset;
}

VS_Output VS(VS_Input input)
{
    VS_Output output;
    output.position = input.position + offset;
    output.uv = input.uv;
    
    return output;
}

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 PS(VS_Output input) : SV_TARGET
{
    float4 color = texture0.Sample(sampler0, input.uv);
    return color;
}