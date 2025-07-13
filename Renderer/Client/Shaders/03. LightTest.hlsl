#include "Global.hlsli"
#include "Light.hlsli"

MeshOutput VS(VertexTangentInput input)
{
    MeshOutput output;
    
    output.position = mul(input.position, BoneTransforms[BoneIndex]);
    output.position = mul(output.position, W);
    output.worldPosition = output.position.xyz;
    output.position = mul(output.position, VP);
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) W);
    output.tangent = mul(input.tangent, (float3x3) W);
    return output;
}

float4 PS(MeshOutput input) : SV_Target
{
    ComputeNormalMapping(input.normal, input.tangent, input.uv);
    
    return ComputeLight(input.normal, input.uv, input.worldPosition);
}