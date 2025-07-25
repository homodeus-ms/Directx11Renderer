#include "Global.hlsli"
#include "Light.hlsli"

VertexClipPosOnlyOutput VS(VertexPosOnlyInput input)
{
    VertexClipPosOnlyOutput output;
    
    float4 worldPos = mul(float4(input.position.xyz, 1.f), W);
    output.clipPosition = mul(worldPos, lightVP[0]); // ¼¨µµ¿ì¸Ê °è»ê½Ã ¸Å¹ø ÇØ´ç VP °»½ÅµÊ
    float3 viewPos = mul(worldPos, lightViews[0]).xyz;
    output.clipPosition.z = length(viewPos) * output.clipPosition.w / 100.f;
    
    return output;
}

// Using Only Point Light
float4 PS(VertexClipPosOnlyOutput input) : SV_Target
{
    float l = input.clipPosition.z;
    return float4(l, 0.f, 0.f, 1.f);
}