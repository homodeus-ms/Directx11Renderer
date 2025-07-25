#include "Global.hlsli"
#include "Light.hlsli"

MeshOutput VS(VertexTangentInput input)
{
    MeshOutput output;
    
    output.position = mul(input.position, W);
    output.worldPosition = output.position.xyz;
    output.position = mul(output.position, VP);
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) W);
    output.tangent = mul(input.tangent, (float3x3) W);
    
    return output;
}

float4 PS(MeshOutput input) : SV_Target
{
    float4 red = { 1.f, 0.f, 0.f, 1.f };
    float4 green = { 0.f, 1.f, 0.f, 1.f };
    float4 blue = { 0.f, 0.f, 1.f, 1.f };
    
    ComputeNormalMapping(input.normal, input.tangent, input.uv);
    input.normal = normalize(input.normal);
    
    float ldotN = dot(-GlobalLight.direction, input.normal);
    float4 color = green;
    float4 specular = red;
    
    [flatten]
    if (ldotN > 0.f)
    {
        color = DiffuseMap.Sample(LinearSampler, input.uv);
        float4 s1 = { 1.f, 1.f, 1.f, 1.f };
        float4 s2 = { 1.f, 1.f, 1.f, 1.f };
        float3 toEye = normalize(CameraPosition - input.worldPosition);
        float3 lightDir = { 0.f, 0.f, 1.f };
        GetSpecular(s1, s2, toEye, lightDir, input.normal, specular);
    }
    
    
    //ComputeSpotLight(SpotLights[0], input.normal, input.uv, input.worldPosition);
    
    return color + specular;
}