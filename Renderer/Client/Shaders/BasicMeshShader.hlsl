#include "Global.hlsli"
#include "Light.hlsli"
#include "GetColorFuncs.Hlsli"

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
    float3 toEye = normalize(CameraPosition - input.worldPosition);
    float3 limLightNormal = normalize(input.normal);
    ComputeNormalMapping(input.normal, input.tangent, input.uv);
    input.normal = normalize(input.normal);
    
    float4 litColor = BLACK;
    bool bUnLit = Material.bUnLit == 0 ? false : true;
    
    if (!bUnLit)
        litColor = CalculateLitColor(input);
    else
        return float4(Material.diffuse.xyz, 1.f);
    
    switch (Material.MaterialType)
    {
        case MATERIAL_TYPE_DEFAULT:
            break;
        case MATERIAL_TYPE_LIM_LIGHT:
            float4 limLight = ComputeRimLight(false, DEFAULT_LIM_LIGHT_COLOR, toEye, limLightNormal);
            litColor += limLight;
            break;
        case MATERIAL_TYPE_TOON:
            return GetToonShadingByGlobalLight(litColor, input.normal, input.uv);
        default:
            return float4(0.f, 1.f, 0.f, 1.f);
    }
    
    return litColor;
}