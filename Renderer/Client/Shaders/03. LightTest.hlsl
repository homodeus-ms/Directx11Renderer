#include "Global.hlsli"
#include "Light.hlsli"

MeshOutput VS(VertexInput input)
{
    MeshOutput output;
    
    output.position = mul(input.position, World);
    output.worldPosition = output.position.xyz;
    output.position = mul(output.position, VP);
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3)World);
    return output;
}

float4 PS(MeshOutput input) : SV_Target
{
    //float4 temp = DiffuseMap.Sample(LinearSampler, input.uv);
    //float4 ambient = ComputeAmbient(input.normal, input.uv);
    //float4 diffuse = ComputeDiffuse(input.normal, input.uv);
    float4 temp = { 1.f, 0.f, 0.f, 1.f };
    return ComputeLight(input.normal, input.uv, input.worldPosition);
}