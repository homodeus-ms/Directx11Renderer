#include "Global.hlsli"
#include "Light.hlsli"
#include "GetColorFuncs.Hlsli"

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
    output.position = mul(pos, VP);
    
    return output;
}

float4 PS(MeshOutput input) : SV_Target
{
    float3 toEye = normalize(CameraPosition - input.worldPosition);
    ComputeNormalMapping(input.normal, input.tangent, input.uv);
    input.normal = normalize(input.normal);
    
    float3 albedo = Material.bUseAlbedoMap ? AlbedoMap.Sample(LinearSampler, input.uv).xyz : Material.diffuse.xyz;
    float AO = Material.bUseAOMap ? AOMap.Sample(LinearSampler, input.uv).r : 0.f;
    float metallic = Material.bUseMetallicRoughnessMap ? MetallicRoughnessMap.Sample(LinearSampler, input.uv).b * Material.metallic: Material.metallic;
    float roughness = Material.bUseMetallicRoughnessMap ? MetallicRoughnessMap.Sample(LinearSampler, input.uv).g * Material.roughness : Material.roughness;
    float3 emissive = Material.bUseEmissiveMap ? EmissiveMap.Sample(LinearSampler, input.uv).xyz : Material.emissive.xyz;
    
    float3 iblColor = { 0.f, 0.f, 0.f };
    float3 litColor = { 0.f, 0.f, 0.f };
    if (Material.MaterialType == MATERIAL_TYPE_PBR)
    {
        iblColor = CalculatePBRIBLLighting(albedo, AO, metallic, roughness, input.normal, toEye);
        litColor = CalculatePBRLitColor(input, albedo, metallic, roughness).xyz;
    }
    else
    {
        iblColor = CalculateDefaultIBLLighting(albedo, input.worldPosition, input.normal, toEye, input.uv);
        litColor = CalculateDefaultLitColor(input, albedo).xyz;
    }
    
    float4 outColor = float4(iblColor + litColor, 1.f);
    outColor = clamp(outColor, 0.0, 1000.0);
    return outColor;
    
    
    switch (Material.MaterialType)
    {
        case MATERIAL_TYPE_DEFAULT:
            break;
        case MATERIAL_TYPE_LIM_LIGHT:
            float4 limLight = ComputeRimLight(false, DEFAULT_LIM_LIGHT_COLOR, toEye, input.normal);
            litColor += limLight;
            break;
        case MATERIAL_TYPE_TOON:
            return GetToonShadingByGlobalLight(litColor, input.normal, input.uv);
        default:
            return float4(0.f, 1.f, 0.f, 1.f);
    }
    
    return float4(litColor, 1.f);
}