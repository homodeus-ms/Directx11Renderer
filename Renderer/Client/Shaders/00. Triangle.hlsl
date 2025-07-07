
struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.position = input.position;
    output.uv = input.uv;
    return output;
}

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 PS(VertexOutput input) : SV_Target
{
    float4 color = texture0.Sample(sampler0, input.uv);
    return color;
}