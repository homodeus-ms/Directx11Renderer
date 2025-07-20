#include "Global.hlsli"



MeshOutput VS(VertexTangentInput input)
{
    MeshOutput output;
    
    output.position = mul(input.position, W);
    output.worldPosition = output.position.xyz;
    output.position = mul(output.position, VP);
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) W);
    output.tangent = mul(input.normal, (float3x3) W);
    
    return output;
}

float4 PS(MeshOutput input) : SV_Target
{
    float4 diffuse = textureCube.Sample(LinearSampler, input.normal);
    diffuse *= Material.diffuse;
    return diffuse;
}