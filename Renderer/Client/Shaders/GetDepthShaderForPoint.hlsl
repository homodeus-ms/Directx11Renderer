#include "Global.hlsli"
#include "Light.hlsli"

struct GSInput
{
    float4 worldPosition : POSITION;
};

struct PSInput
{
    float3 worldPosition : Position;
    float4 clipPosition : SV_Position;
    uint renderTargetIndex : SV_RenderTargetArrayIndex;
};

GSInput VS(VertexPosOnlyInput input)
{
    GSInput output;
    output.worldPosition = mul(float4(input.position.xyz, 1.f), W);
    
    return output;
}

[maxvertexcount(18)]
void GS(triangle GSInput input[3], inout TriangleStream<PSInput> stream)
{
    [unroll]
    for (int i = 0; i < 6; ++i)
    {
        PSInput output[3];
        
        for (int j = 0; j < 3; ++j)
        {
            output[j].worldPosition = input[j].worldPosition.xyz;
            output[j].clipPosition = mul(input[j].worldPosition, g_PointLightVPs[i]);
            output[j].renderTargetIndex = i;
        }

        stream.Append(output[0]);
        stream.Append(output[1]);
        stream.Append(output[2]);
        stream.RestartStrip();
    }
}

float4 PS(PSInput input) : SV_Target
{
    float l = length(input.worldPosition.xyz - g_ShadowedPointLightPosition) / 100.f;
    return float4(l, 0.f, 0.f, 1.f);
}

//VertexClipPosOnlyOutput VS(VertexPosOnlyInput input)
//{
//    VertexClipPosOnlyOutput output;
//    
//    float4 worldPos = mul(float4(input.position.xyz, 1.f), W);
//    output.clipPosition = mul(worldPos, lightVP[0]); // ¼¨µµ¿ì¸Ê °è»ê½Ã ¸Å¹ø ÇØ´ç VP °»½ÅµÊ
//    
//    return output;
//}

// Using Only Point Light
//float4 PS(VertexClipPosOnlyOutput input) : SV_Target
//{
//    // TEMP : Generate Shadow Cube Map Only 1
//    float l = length(input.worldPosition.xyz - PointLights[0].position) / 100.f;
//    //float l = input.clipPosition.z;
//    return float4(l, 0.f, 0.f, 1.f);
//}
