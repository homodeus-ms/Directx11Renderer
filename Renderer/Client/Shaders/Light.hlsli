#ifndef LIGHT_HLSLI
#define LIGHT_HLSLI

#include "Global.hlsli"

#define SPECULAR_INTENSITY (20)
#define EMISSIVE_INTENSITY (4)

struct DirectionalLightDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    float3 color;
    float radiance;
    float radianceFactor;
    
    float3 direction;
    int shadowMapIndex;
    uint isOn;
    float2 directionalPad;
};

struct SpotLightDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    float3 color;
    float radiance;
    float radianceFactor;
    
    float3 position;
    int shadowMapIndex;
    float3 direction;
    float range;
    float3 attenuation; // constant, linear, quadratic
    float spotPower;
    uint isOn;
    float2 spotPad;
};

struct PointLightDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    float3 color;
    float radiance;
    float radianceFactor;
    
    float3 position;
    uint bShadowMapUsing;
    float3 attenuation; // constant, linear, quadratic
    float range;
    uint isOn;
    float2 pointPad;
};

cbuffer DirectionalLightBuffer : register(CBUFFER_NUM_DIRECTIONAL_LIGHT)
{
    DirectionalLightDesc GlobalLight;
}

cbuffer SpotLightBuffer : register(CBUFFER_NUM_SPOT_LIGHT)
{
    SpotLightDesc SpotLights[MAX_SPOT_LIGHT_COUNT];
    uint SpotlightCount; 
    float3 spotLight_padding;
}

cbuffer PointLightBuffer : register(CBUFFER_NUM_POINT_LIGHT)
{
    PointLightDesc PointLights[MAX_POINT_LIGHT_COUNT];
    uint PointlightCount;
    float3 pointLight_padding;
}

struct LightCalcParams
{
    float3 worldNormal;
    float mipLevel;
    float2 uv;
    float metallic;
    float roughness;
    float3 worldPosition;
    float pad1;
    float3 albedo;
    float pad2;
};

// From Unreal4 PBR

static const float3 g_Fdielectric = float3(0.04f, 0.04f, 0.04f); // 비 금속 재질 Fresnel 0
float NdfGGX(float NdotH, float roughness);

// Single term for separable Schlick-GGX below.
float SchlickG1(float NdotV, float k);

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float SchlickGGX(float NdotI, float NdotO, float roughness);

float3 SchlickFresnel(float3 F0, float VDotH)
{
    return F0 + (1.0 - F0) * exp2((-5.55473f * VDotH - 6.98316f) * VDotH);
}

// Compute Light Functions
void GetSpecular(float4 lightSpec, float4 matSpec, float3 toEye, float3 lightDir, float3 normal, out float3 specular)
{
    float3 R = normalize(reflect(lightDir, normal));
    float RDotEye = saturate(dot(R, toEye));
    
    float factor = pow(RDotEye, SPECULAR_INTENSITY);
    specular = (lightSpec * matSpec * factor).xyz;
}

float4 ComputeRimLight(bool useLight, float4 matE, float3 toEye, float3 normal)
{
    float4 emissive = BLACK;
    
    float eyeDotNormal = saturate(dot(toEye, normal));
    float factor = 1.f - eyeDotNormal;
    factor = smoothstep(0.0f, 1.0f, factor);
    factor = pow(factor, EMISSIVE_INTENSITY);
    
    if (!useLight)
    {
        emissive = matE * factor;
        return emissive;
    }
    
    float4 E = matE * factor;
    
    emissive = GlobalLight.emissive * E;
    
    for (uint i = 0; i < SpotlightCount; ++i)
    {
        emissive += SpotLights[i].emissive * E;
    }
    for (uint j = 0; j < PointlightCount; ++j)
    {
        emissive += PointLights[j].emissive * E;
    }
    
    return emissive;
}

float ComputeShadowFactor(float3 worldPosition, uint index, float bias)
{
    if (index >= MAX_ACTIVE_SHADOW_LIGHT)
        return 0.f;
    
    float4 clipPosition = mul(float4(worldPosition.xyz, 1.f), g_LightVP[index]);
    float3 clipCoord = clipPosition.xyz / clipPosition.w;
    float2 uv = clipCoord.xy;
    uv.y = -uv.y;
    uv = (uv * 0.5f) + 0.5f;
    float currentDepth = clipCoord.z;
    
    float shadowFactor = 0.f;
    
    if (index == 0)
        shadowFactor = ShadowMaps[0].SampleCmpLevelZero(ComparisionSampler, uv, currentDepth - bias).r;
    else if (index == 1)
        shadowFactor = ShadowMaps[1].SampleCmpLevelZero(ComparisionSampler, uv, currentDepth - bias).r;
    else if (index == 2) 
        shadowFactor = ShadowMaps[2].SampleCmpLevelZero(ComparisionSampler, uv, currentDepth - bias).r;
    else
        shadowFactor = ShadowMaps[3].SampleCmpLevelZero(ComparisionSampler, uv, currentDepth - bias).r;
    
    return pow(shadowFactor, 10);
}

float ComputePointLightShadowFactor(float3 lightPos, float3 worldPosition, float bias)
{
    float3 lightToPixel = worldPosition - lightPos;
    float currentDepth = length(lightToPixel) / 100.f;
    
    //float shadowDepth = ShadowCubeMap.Sample(LinearSampler, normalize(lightToPixel)).r;
    //float shadowFactor = 1.f;
    //if (currentDepth > shadowDepth - bias)
    //    shadowFactor = 0.0f;
    
    float shadowFactor = ShadowCubeMap.SampleCmpLevelZero(ComparisionSampler, normalize(lightToPixel), currentDepth - bias).r;
    
    return shadowFactor;
}

float4 ComputeDirectionalLight(float3 normal, float2 uv, float3 worldPosition, float mipLevel)
{
    if (GlobalLight.isOn == 0)
        return float4(0.f, 0.f, 0.f, 0.f);
    
    float3 ambient = { 0.f, 0.f, 0.f };
    float3 diffuse = { 0.f, 0.f, 0.f };
    float3 specular = { 0.f, 0.f, 0.f };
 
    float3 albedo = AlbedoMap.SampleLevel(LinearSampler, uv, mipLevel).xyz;
    
    // ambient
    float3 ambientFactor = GlobalLight.ambient.xyz * Material.ambient.xyz;
    ambient = albedo * ambientFactor;
    
    // diffuse, specular
    float3 toEye = normalize(CameraPosition - worldPosition);
    float lDotN = dot(-GlobalLight.direction, normal);
    
    [flatten]
    if (lDotN > 0)
    {
        float3 diffuseFactor = GlobalLight.diffuse.xyz * Material.diffuse.xyz * lDotN;
        diffuse = albedo * diffuseFactor;
        GetSpecular(GlobalLight.specular, Material.specular, toEye, GlobalLight.direction, normal, specular);
    }
    
    int shadowMapIndex = GlobalLight.shadowMapIndex;
    float shadowFactor = 1.f;
    if (shadowMapIndex != -1)
    {
        shadowFactor = ComputeShadowFactor(worldPosition, (uint) shadowMapIndex, 0.001f);
    }
    
    return float4(ambient + (diffuse + specular) * shadowFactor, 1.f);
}

float4 ComputeDefaultSpotLight(SpotLightDesc L, LightCalcParams params)
{
    if (L.isOn = 0)
        return float4(0.f, 0.f, 0.f, 0.f);
    
    float3 toLightVec = L.position - params.worldPosition;
    
    float d = length(toLightVec);
    
    if (d > L.range)
        return float4(0.f, 0.f, 0.f, 0.f);
    
    toLightVec = toLightVec / d; // normalize
    
    float3 ambient = { 0.f, 0.f, 0.f };
    float3 diffuse = { 0.f, 0.f, 0.f };
    float3 specular = { 0.f, 0.f, 0.f };
    
    // Ambient
    float3 albedo = params.albedo;
    ambient = albedo * L.ambient.xyz * Material.ambient.xyz;
    
    // Diffuse
    float3 toEye = normalize(CameraPosition - params.worldPosition);
    float lDotN = dot(toLightVec, params.worldNormal);
    
    [flatten]
    if (lDotN > 0.f)
    {
        float3 diffuseFactor = L.diffuse.xyz * Material.diffuse.xyz * lDotN;
        diffuse = albedo * diffuseFactor;
    
        // Specular
        GetSpecular(L.specular, Material.specular, toEye, -toLightVec, params.worldNormal, specular);
    }
    
    // SpotPower
    float spot = pow(max(dot(-toLightVec, L.direction), 0.0f), L.spotPower);
    float att = spot / dot(L.attenuation, float3(1.0f, d, d * d));
    
    ambient *= spot;
    diffuse *= att * 3.f;
    specular *= att * 3.f;
    
    int shadowMapIndex = L.shadowMapIndex;
    float shadowFactor = 1.f;
    if (shadowMapIndex != -1)  
    {
        shadowFactor = ComputeShadowFactor(params.worldPosition, (uint) shadowMapIndex, 0.01);
    }
    
    return float4(ambient + (diffuse + specular) * shadowFactor, 1.f);
}

float4 ComputePBRSpotLight(SpotLightDesc L, LightCalcParams params)
{
    float3 toLight = L.position - params.worldPosition;
    float d = length(toLight);
    toLight = normalize(toLight);
    float3 toEye = normalize(CameraPosition - params.worldPosition);
    float3 halfway = normalize(toEye + toLight);
    
    if (d > L.range)
        return BLACK;
    
    float NDotL = saturate(dot(params.worldNormal, toLight));
    float NDotH = saturate(dot(params.worldNormal, halfway));
    float NDotE = saturate(dot(params.worldNormal, toEye));
    
    const float3 Fdielectric = 0.04; // 비금속(Dielectric) 재질의 F0
    float3 F0 = lerp(Fdielectric, params.albedo, params.metallic);
    float3 F = SchlickFresnel(F0, max(0.0, dot(halfway, toEye)));
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), params.metallic);
    float3 diffuse = kd * params.albedo;

    float D = NdfGGX(NDotH, params.roughness);
    float3 G = SchlickGGX(NDotL, NDotE, params.roughness);
    float3 specular = (F * D * G) / max(1e-5, 4.0 * NDotL * NDotE);
    
    // Attenuate
    float spot = pow(max(dot(-toLight, L.direction), 0.0f), L.spotPower);
    float att = spot / dot(L.attenuation, float3(1.0f, d, d * d));
    diffuse *= att;
    specular *= att;
    
    int shadowMapIndex = L.shadowMapIndex;
    float shadowFactor = 1.f;
    if (shadowMapIndex != -1)
    {
        shadowFactor = ComputeShadowFactor(params.worldPosition, (uint) shadowMapIndex, 0.01);
    }
    
    float3 sum = (diffuse + specular) * L.radiance * L.radianceFactor * NDotL * shadowFactor;
    return float4(sum, 1.f);
}

float4 ComputeDefaultPointLight(PointLightDesc L, LightCalcParams params)
{
    if (L.isOn == 0)
        return RED;
    
    float3 toLightVec = L.position - params.worldPosition;
    float d = length(toLightVec);
    
    if (d > L.range)
        return float4(0.f, 0.f, 0.f, 0.f);
    
    toLightVec = toLightVec / d; // normalize
    
    float3 ambient = { 0.f, 0.f, 0.f };
    float3 diffuse = { 0.f, 0.f, 0.f };
    float3 specular = { 0.f, 0.f, 0.f };
   
    // Ambient
    float3 albedo = params.albedo;
    ambient = albedo * L.ambient.xyz * Material.ambient.xyz;
    
    // Diffuse
    float lDotN = dot(toLightVec, params.worldNormal);
    float3 toEye = normalize(CameraPosition - params.worldPosition);

    [flatten]
    if (lDotN > 0.f)
    {
        float3 diffuseFactor = L.diffuse.xyz * Material.diffuse.xyz * lDotN;
        diffuse = albedo * diffuseFactor;
        GetSpecular(L.specular, Material.specular, toEye, -toLightVec, params.worldNormal, specular);
    }
    
    // Attenuate
    float att = 1.0f / dot(L.attenuation, float3(1.0f, d, d * d));
    
    ambient *= (att * 5.f);
    diffuse *= att;
    specular *= att;
    
    float shadowFactor = 1.f;
    if (L.bShadowMapUsing)
    {
        shadowFactor = ComputePointLightShadowFactor(L.position, params.worldPosition, 0.1f);
    }
    
    return float4(ambient + (diffuse + specular) * shadowFactor, 1.f);
}

float4 ComputePBRPointLight(PointLightDesc L, LightCalcParams params)
{
    float3 toLight = L.position - params.worldPosition;
    float d = length(toLight);
    toLight = normalize(toLight);
    float3 toEye = normalize(CameraPosition - params.worldPosition);
    float3 halfway = normalize(toEye + toLight); 
    
    if (d > L.range)
        return BLACK;
    
    float NDotL = saturate(dot(params.worldNormal, toLight));
    float NDotH = saturate(dot(params.worldNormal, halfway));
    float NDotE = saturate(dot(params.worldNormal, toEye));
    
    const float3 Fdielectric = 0.04;     // 비금속(Dielectric) 재질의 F0
    float3 F0 = lerp(Fdielectric, params.albedo, params.metallic);
    float3 F = SchlickFresnel(F0, max(0.0, dot(halfway, toEye)));
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), params.metallic);
    float3 diffuse = kd * params.albedo;

    float D = NdfGGX(NDotH, params.roughness);
    float3 G = SchlickGGX(NDotL, NDotE, params.roughness);
    float3 specular = (F * D * G) / max(1e-5, 4.0 * NDotL * NDotE);
    
    // Attenuate
    float att = 1.0f / dot(L.attenuation, float3(1.0f, d, d * d));
    diffuse *= att;
    specular *= att;
    
    float shadowFactor = 1.f;
    if (L.bShadowMapUsing == 1)
    {
        shadowFactor = ComputePointLightShadowFactor(L.position, params.worldPosition, 0.1f);
    }
    
    float3 sum = (diffuse + specular) * L.radiance * L.radianceFactor * NDotL * shadowFactor;
    return float4(sum, 1.f);
}

void ComputeNormalMapping(inout float3 worldNormal, float3 worldTangent, float2 uv)
{
    if (Material.bUseNormalMap == 0)
        return;
    
    // 샘플링 결과는 [0, 1] 범위
    float4 sampledNormal = NormalMap.Sample(LinearSampler, uv);
    
    if (any(sampledNormal.rgb) == false)
        return;

    // TBN
    float3 N = normalize(worldNormal); // Normal (Z axis)
    float3 T = normalize(worldTangent); // Tangent (X axis)
    float3 B = normalize(cross(N, T)); // Bitangent (Y axis) - 순서 주의: N x T
    float3x3 tangentToWorld = float3x3(T, B, N);

    // 텍스처 RGB [0,1] → [-1,1]로 변환
    float3 tangentSpaceNormal = sampledNormal.rgb * 2.0f - 1.0f;

    // 탄젠트 스페이스 노멀 → 월드 스페이스 노멀
    worldNormal = normalize(mul(tangentSpaceNormal, tangentToWorld));
}

float4 CalculateDefaultLitColor(in MeshOutput input, float3 albedo)
{
    float mipLevel = GetMipLevel(input.worldPosition);
    
    // TEMP : Directional Light 사용 X
    float4 directionalColor = ComputeDirectionalLight(input.normal, input.uv, input.worldPosition, mipLevel);
    directionalColor = float4(0.f, 0.f, 0.f, 0.f);
    
    LightCalcParams params;
    params.worldNormal = input.normal;
    params.mipLevel = mipLevel;
    params.uv = input.uv;
    params.worldPosition = input.worldPosition;
    params.albedo = albedo;
    
    float4 spotColor = { 0.0f, 0.0f, 0.0f, 1.f };
    for (uint i = 0; i < SpotlightCount; ++i)
    {
        spotColor += ComputeDefaultSpotLight(SpotLights[i], params);
        
    }
    
    float4 pointColor = { 0.f, 0.f, 0.f, 0.f };
    for (uint j = 0; j < PointlightCount; ++j)
    {
        pointColor += ComputeDefaultPointLight(PointLights[j], params);
    }
    
    float4 emissiveColor = { 0.f, 0.f, 0.f, 0.f };
    if (Material.bUseEmissiveMap)
    {
        emissiveColor = EmissiveMap.Sample(LinearSampler, input.uv);
    }
    
    float4 color = directionalColor + spotColor + pointColor + emissiveColor;
    color.w = 1.f;
    
    return color;
}

float4 CalculatePBRLitColor(in MeshOutput input, float3 albedo, float metallic, float roughness)
{
    float mipLevel = GetMipLevel(input.worldPosition);
    
    // TEMP : Directional Light 사용 X
    //float4 directionalColor = ComputeDirectionalLight(input.normal, input.uv, input.worldPosition, mipLevel);
    float4 directionalColor = { 0.f, 0.f, 0.f, 0.f };
    
    LightCalcParams params;
    params.worldNormal = input.normal;
    params.mipLevel = mipLevel;
    params.uv = input.uv;
    params.metallic = metallic;
    params.roughness = roughness;
    params.worldPosition = input.worldPosition;
    params.albedo = albedo;
    
    float4 spotColor = { 0.0f, 0.0f, 0.0f, 1.f };
    for (uint i = 0; i < SpotlightCount; ++i)
    {
        spotColor += ComputePBRSpotLight(SpotLights[i], params);
    }
    
    float4 pointColor = { 0.f, 0.f, 0.f, 0.f };
    for (uint j = 0; j < PointlightCount; ++j)
    {
        pointColor += ComputePBRPointLight(PointLights[j], params);
    }
    
    float4 emissiveColor = { 0.f, 0.f, 0.f, 0.f };
    if (Material.bUseEmissiveMap)
    {
        emissiveColor = EmissiveMap.Sample(LinearSampler, input.uv);
    }
    
    float4 color = directionalColor + spotColor + pointColor + emissiveColor;
    color.w = 1.f;
    
    return color;
}

float NdfGGX(float NdotH, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;
    float denom = (NdotH * NdotH) * (alphaSq - 1.f) + 1.f;
    denom = denom + EPSILON;
    
    return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float SchlickG1(float NdotV, float k)
{
    float denom = (NdotV * (1.f - k) + k) + EPSILON;
    return NdotV / denom;
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float SchlickGGX(float NdotI, float NdotO, float roughness)
{
    float r = roughness + 1.f;
    float k = (r * r) / 8.f;
    return SchlickG1(NdotI, k) * SchlickG1(NdotO, k);
}

float3 CalculateDefaultIBLLighting(float3 albedo, float3 worldPosition, float3 worldNormal, float3 toEye, float2 uv)
{
    if (Material.bGetIBL == 0)
        return float3(0.f, 0.f, 0.f);
    
    float3 viewR = reflect(-toEye, worldNormal);
    float4 envSpec = IBLSpec.Sample(LinearSampler, viewR);
    envSpec *= pow((envSpec.r + envSpec.g + envSpec.b) / 3.f, 3.f);
    envSpec.xyz *= Material.specular.xyz;
        
    float4 envDiff = IBLDiff.Sample(LinearSampler, worldNormal);
    envDiff.xyz *= Material.diffuse.xyz;
    float mipLevel = GetMipLevel(worldPosition);
    envDiff *= AlbedoMap.SampleLevel(LinearSampler, uv, mipLevel);
        
    float4 envColor = envSpec + envDiff;
    
    return envColor.xyz * 0.2f;
}

float3 CalculatePBRIBLLighting(float3 albedo, float AO, float metallic, float roughness, float3 worldNormal, float3 toEye)
{
    if (Material.bGetIBL == 0)
        return float3(0.f, 0.f, 0.f);
    
    // Diffuse IBL
    float3 F0 = lerp(g_Fdielectric, albedo, metallic);
    float3 F = SchlickFresnel(F0, max(0.f, dot(worldNormal, toEye)));
    float3 kd = lerp(1.0 - F, 0.f, metallic);
    float3 irradiance = IBLDiff.SampleLevel(LinearSampler, worldNormal, 0.f).rgb;
    
    float3 diffuse = kd * albedo * irradiance;
    
    // Specular IBL
    float2 brdf = BRDFMap.SampleLevel(ClampSampler, float2(dot(worldNormal, toEye), 1.0 - roughness), 0.f).rg;
    float3 specularIrradiance = IBLSpec.SampleLevel(LinearSampler, reflect(-toEye, worldNormal),
                                                            3 + roughness * 5.0f).rgb;
    F0 = lerp(g_Fdielectric, albedo, metallic);
    
    float3 specular = (F0 * brdf.x + brdf.y) * specularIrradiance;
    
    return (diffuse + specular) * AO;
}

#endif /* LIGHT_HLSLI */