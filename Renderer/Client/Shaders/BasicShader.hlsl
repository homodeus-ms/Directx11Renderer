#include "Global.hlsli"
#include "Light.hlsli"
#include "GetColorFuncs.hlsli"

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
    float3 toEye = normalize(CameraPosition - input.worldPosition);
    float3 inputNormal = normalize(input.normal);
    ComputeNormalMapping(input.normal, input.tangent, input.uv);
    input.normal = normalize(input.normal);
    
    float4 litColor = BLACK;
    bool bUnLit = Material.bUnLit == 0 ? false : true;
    
    if (!bUnLit)
        litColor = CalculateLitColor(input);
    
    return litColor;
    
    int matType = Material.MaterialType;
    switch (matType)
    {
        case MATERIAL_TYPE_DEFAULT:
            break; 
        case MATERIAL_TYPE_LIM_LIGHT:
            float4 limLight = ComputeRimLight(false, DEFAULT_LIM_LIGHT_COLOR, toEye, inputNormal);
            litColor += limLight;
            break;
        case MATERIAL_TYPE_TOON:
            return GetToonShadingByGlobalLight(litColor, input.normal, input.uv);
        default:
            return float4(0.f, 1.f, 0.f, 1.f);
    }
    
    // Env Lighting
    if (bEnvLightUsing == 1)
    {
        inputNormal = float3(0.f, 0.f, 1.f);
        float3 viewR = reflect(-toEye, inputNormal);
        float4 envColor = textureCube.Sample(LinearSampler, normalize(input.worldPosition));
        litColor = litColor * 0.7f + envColor * 0.3f;
        return float4(litColor.xyz, 1.f); 
    }
    
    return litColor;
    
}