#include "Global.hlsli"

struct CubeMapInput
{
    float4 position : POSITION;
};

struct CubeMapOutput
{
    float4 position : SV_Position;
    float3 worldPosition : TEXCOORD;
};

CubeMapOutput VS(CubeMapInput input)
{
    CubeMapOutput output;
    
    output.worldPosition = input.position.xyz;
    
    input.position.w = 0.f;
    float4 pos = mul(input.position, V);
    pos = mul(float4(pos.xyz, 1.f), P);
    
    output.position = pos;
    
    return output;
}

float4 PS(CubeMapOutput input) : SV_Target
{
    float4 color = textureCube.Sample(LinearSampler, input.worldPosition);
    return color;
}