#include "Global.hlsli"
#include "Light.hlsli"

cbuffer LightVPIndex : register(b0)
{
    uint g_LightIndex;
    float3 LightVPIndexPad;
};

//VertexClipPosOnlyOutput VS(VertexPosOnlyInput input)
//{
//    VertexClipPosOnlyOutput output;
//    
//    float4 worldPosition = mul(float4(input.position.xyz, 1.f), W);
//    output.clipPosition = mul(worldPosition, g_LightVP[g_LightIndex]);
//    
//    return output;
//}

MeshOutput VS(VertexTangentInput input)
{
    MeshOutput output;
    
    float4 pos = mul(input.position, W);
    
    output.uv = input.uv;
   
    output.normal = mul(float4(input.normal, 0.f), invW).xyz;
    output.normal = normalize(output.normal);
    output.tangent = mul(input.tangent, (float3x3) W);
    
    if (Material.bUseHeightMap == 1)
    {
        float height = HeightMap.SampleLevel(LinearSampler, input.uv, 0).r;
        height = height * 2.f - 1.f;
        pos += float4(output.normal * g_heightScale * height, 0.f);
    }
    
    output.worldPosition = pos.xyz;
    output.position = mul(pos, g_LightVP[g_LightIndex]);
    
    return output;
}