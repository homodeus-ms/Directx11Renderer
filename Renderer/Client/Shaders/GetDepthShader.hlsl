#include "Global.hlsli"
#include "Light.hlsli"

cbuffer LightVPIndex : register(b0)
{
    uint g_LightIndex;
    float3 LightVPIndexPad;
};

VertexClipPosOnlyOutput VS(VertexPosOnlyInput input)
{
    VertexClipPosOnlyOutput output;
    
    float4 worldPosition = mul(float4(input.position.xyz, 1.f), W);
    output.clipPosition = mul(worldPosition, g_LightVP[g_LightIndex]);
    
    return output;
}
