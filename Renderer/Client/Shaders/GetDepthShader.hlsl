#include "Global.hlsli"
#include "Light.hlsli"

struct PSInput
{
    float4 clipPosition : SV_Position;
    uint arrayIndex : SV_RenderTargetArrayIndex;
};

struct GSInput
{
    float4 clipPosition : SV_Position;
    uint arrayIndex : ARRAYINDEX;
};

GSInput VS(VertexPosOnlyInput input, uint instanceID : SV_InstanceID)
{
    GSInput output;
    float4 worldPosition = mul(float4(input.position.xyz, 1.f), W);
    output.clipPosition = mul(worldPosition, g_LightVP[instanceID]);
    output.arrayIndex = instanceID;
    return output;
}

[maxvertexcount(3)]
void GS(triangle GSInput input[3], inout TriangleStream<PSInput> stream)
{
    PSInput output[3];
    
    [unroll]
    for (int i = 0; i < 3; ++i)
    {
        output[i].clipPosition = input[i].clipPosition;
        output[i].arrayIndex = input[i].arrayIndex;
    }

    stream.Append(output[0]);
    stream.Append(output[1]);
    stream.Append(output[2]);
    stream.RestartStrip();
}

float4 PS(PSInput input) : SV_Target
{
    return float4(1.f, 0.f, 0.f, 1.f);
}

