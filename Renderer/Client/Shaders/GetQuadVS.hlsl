#include "Global.hlsli"

VertexUVOutput VS(uint id : SV_VertexID)
{
    float2 pos[6] =
    {
        float2(-1.0, -1.0),
        float2(-1.0, 1.0),
        float2(1.0, 1.0),
        float2(-1.0, -1.0),
        float2(1.0, 1.0),
        float2(1.0, -1.0)
    };

    float2 uv[6] =
    {
        float2(0.0, 1.0),
        float2(0.0, 0.0),
        float2(1.0, 0.0),
        float2(0.0, 1.0),
        float2(1.0, 0.0),
        float2(1.0, 1.0)
    };
    
    VertexUVOutput output;
    output.position = float4(pos[id], 0.f, 1.f);
    output.uv = uv[id];
    
    return output;
}