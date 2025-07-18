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
    input.normal = normalize(input.normal);
    
    float4 directionalColor = ComputeDirectionalLight(input.normal, input.uv, input.worldPosition);
    
    float4 spotColor = { 0.0f, 0.0f, 0.0f, 1.f };
    for (uint i = 0; i < SpotlightCount; ++i)
    {
        spotColor += ComputeSpotLight(SpotLights[i], input.normal, input.uv, input.worldPosition);
    }
    
    float4 pointColor = { 0.f, 0.f, 0.f, 0.f };
    for (uint j = 0; j < PointlightCount; ++j)
    {
        pointColor += ComputePointLight(PointLights[j], input.normal, input.uv, input.worldPosition);
    }
    
    float4 color = directionalColor + spotColor + pointColor;
    color.w = DiffuseMap.Sample(LinearSampler, input.uv).w;
    
    return color;
}