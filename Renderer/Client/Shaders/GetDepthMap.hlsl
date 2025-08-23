#include "Global.hlsli"
#include "Light.hlsli"

VertexClipPosOnlyOutput VS(VertexPosOnlyInput input)
{
    VertexClipPosOnlyOutput output;
    
    float4 worldPosition = mul(float4(input.position.xyz, 1.f), W);
    output.clipPosition = mul(worldPosition, VP);
    
    return output;
}
