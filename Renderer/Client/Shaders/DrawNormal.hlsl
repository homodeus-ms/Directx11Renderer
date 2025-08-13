
#include "Global.hlsli"

#define NormalScale 0.5f

struct GSInput
{
    float4 position : Position;
    float3 normal : NORMAL;
};

struct PSInput
{
    float4 position : SV_Position;
    float3 color : COLOR;
};

GSInput VS(VertexTangentInput input)
{
    GSInput output;
    output.position = input.position;
    output.normal = input.normal;
	return output;
}

[maxvertexcount(2)]
void GS(point GSInput input[1], inout LineStream<PSInput> outStream)
{
    float4 worldPosition = mul(input[0].position, W);
    float4 worldNormal = float4(input[0].normal, 0.f);
    worldNormal = mul(worldNormal, invW);
    worldNormal = float4(normalize(worldNormal).xyz, 0.f);

    PSInput output;
    
    output.position = mul(worldPosition, VP);
    output.color = float3(1.f, 0.f, 0.f);
    outStream.Append(output);

    worldPosition += NormalScale * worldNormal;
    output.position = mul(worldPosition, VP);
    output.color = float3(1.f, 1.f, 1.f);
    outStream.Append(output);
}

float4 PS(PSInput input) : SV_Target
{
    return float4(input.color, 1.f);
}