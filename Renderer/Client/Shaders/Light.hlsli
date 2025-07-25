#ifndef LIGHT_HLSLI
#define LIGHT_HLSLI

#include "Global.hlsli"

#define SPECULAR_INTENSITY (30)
#define EMISSIVE_INTENSITY (4)

struct DirectionalLightDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    int shadowMapIndex;
    float3 pad;
    
    float3 direction;
    uint isOn;
};

struct SpotLightDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    int shadowMapIndex;
    float3 pad;
    
    float3 position;
    float range;
    float3 direction;
    float spotPower;
    float3 attenuation; // constant, linear, quadratic
    uint isOn;
};

struct PointLightDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    int shadowMapIndex;
    float3 pad;
    
    float3 position;
    float range;
    float3 attenuation; // constant, linear, quadratic
    uint isOn;
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

// Compute Light Functions
void GetSpecular(float4 lightSpec, float4 matSpec, float3 toEye, float3 lightDir, float3 normal, out float4 specular)
{
    float3 R = normalize(lightDir - (2 * normal * dot(lightDir, normal)));
    float RDotEye = saturate(dot(R, toEye));
    
    float factor = pow(RDotEye, SPECULAR_INTENSITY);
    specular = lightSpec * matSpec * factor;
}

void GetSpecular2(float4 lightSpec, float4 matSpec, float3 toEye, float3 lightDir, float3 normal, out float4 specular)
{
    float3 R = normalize(reflect(lightDir, normal));
    float RDotEye = saturate(dot(R, toEye));
    
    float factor = pow(RDotEye, SPECULAR_INTENSITY);
    specular = lightSpec * matSpec * factor;
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
    // Temp : Texture2D -> Texture2DArray
    if (index > 3)
        return 0.f;
    
    float4 clipPosition = mul(float4(worldPosition.xyz, 1.f), lightVP[index]);
    float3 clipCoord = clipPosition.xyz / clipPosition.w;
    float2 uv = clipCoord.xy;
    uv.y = -uv.y;
    uv = (uv * 0.5f) + 0.5f;
    float currentDepth = clipCoord.z;
    
    float4 sampled;
    if (index == 0)
        sampled = ShadowMaps[0].Sample(LinearSampler, uv);
    else if (index == 1)
        sampled = ShadowMaps[1].Sample(LinearSampler, uv);
    else if (index == 2)
        sampled = ShadowMaps[2].Sample(LinearSampler, uv);
    else if (index == 3)
        sampled = ShadowMaps[3].Sample(LinearSampler, uv);
    
    float shadowDepth = sampled.r;
    float shadowFactor = currentDepth > shadowDepth + bias ? 0.6f : 1.0f;
    
    return shadowFactor;
}

float ComputePointLightShadowFactor(float3 lightPos, float3 worldPosition, float bias)
{
    float3 lightToPixel = worldPosition - lightPos;
    float currentDepth = length(lightToPixel) / 100.f;
    
    float shadowDepth = ShadowCubeMap.Sample(LinearSampler, normalize(lightToPixel)).r;
    
    float shadowFactor = 1.f;
    
    if (currentDepth > shadowDepth - bias)
        shadowFactor = 0.5f;
    
    return shadowFactor;
}

float4 ComputeDirectionalLight(float3 normal, float2 uv, float3 worldPosition)
{
    if (GlobalLight.isOn == 0)
        return float4(0.f, 0.f, 0.f, 0.f);
    
    float4 ambient = { 0.f, 0.f, 0.f, 0.f };
    float4 diffuse = { 0.f, 0.f, 0.f, 0.f };
    float4 specular = { 0.f, 0.f, 0.f, 0.f };
    float4 emissive = { 0.f, 0.f, 0.f, 0.f };
 
    float4 sampledColor = DiffuseMap.Sample(LinearSampler, uv);
    
    // ambient
    float4 ambientFactor = GlobalLight.ambient * Material.ambient;
    ambient = sampledColor * ambientFactor;
    
    // diffuse, specular
    float3 toEye = normalize(CameraPosition - worldPosition);
    float lDotN = dot(-GlobalLight.direction, normal);
    
    [flatten]
    if (lDotN > 0)
    {
        float4 diffuseFactor = GlobalLight.diffuse * Material.diffuse * lDotN;
        diffuse = sampledColor * diffuseFactor;
        GetSpecular(GlobalLight.specular, Material.specular, toEye, GlobalLight.direction, normal, specular);
    }
    
    int shadowMapIndex = GlobalLight.shadowMapIndex;
    float shadowFactor = 1.f;
    if (bShadowMapUsing == 1 && shadowMapIndex != -1)
    {
        shadowFactor = ComputeShadowFactor(worldPosition, (uint) shadowMapIndex, 0.001f);
    }
    
    //shadowFactor *= 0.8;
    
    return (ambient + diffuse + specular) * shadowFactor;
}

float4 ComputeSpotLight(SpotLightDesc L, float3 normal, float2 uv, float3 worldPosition)
{
    if (L.isOn == 0)
        return RED;
    
    float3 toLightVec = L.position - worldPosition;
    
    float d = length(toLightVec);
    
    if (d > L.range)
        return float4(0.f, 0.f, 0.f, 0.f);
    if (d < 0.01f)
        return float4(1.f, 1.f, 1.f, 1.f);
    
    toLightVec = toLightVec / d; // normalize
    
    float4 ambient = { 0.f, 0.f, 0.f, 0.f };
    float4 diffuse = { 0.f, 0.f, 0.f, 0.f };
    float4 specular = { 0.f, 0.f, 0.f, 0.f };
    float4 emissive = { 0.f, 0.f, 0.f, 0.f };
   
    // Ambient
    float4 sampledColor = DiffuseMap.Sample(LinearSampler, uv);
    ambient = sampledColor * L.ambient * Material.ambient;
    
    // Diffuse
    float3 toEye = normalize(CameraPosition - worldPosition);
    float lDotN = dot(toLightVec, normal);
    
    [flatten]
    if (lDotN > 0.f)
    {
        float4 diffuseFactor = L.diffuse * Material.diffuse * lDotN;
        diffuse = sampledColor * diffuseFactor;
    
        // Specular
        GetSpecular(L.specular, Material.specular, toEye, -toLightVec, normal, specular);
    }
    
    // SpotPower
    float spot = pow(max(dot(-toLightVec, L.direction), 0.0f), L.spotPower);
    float att = spot / dot(L.attenuation, float3(1.0f, d, d * d));
    
    ambient *= spot;
    diffuse *= att;
    specular *= att;
    
    int shadowMapIndex = L.shadowMapIndex;
    
    float shadowFactor = 1.f;
    if (bShadowMapUsing == 1 && shadowMapIndex != -1)
    {
        shadowFactor = ComputeShadowFactor(worldPosition, (uint) shadowMapIndex, 0.01);
    }
    
    return (ambient + diffuse + specular) * shadowFactor;
}

float4 ComputePointLight(PointLightDesc L, float3 normal, float2 uv, float3 worldPosition)
{
    if (L.isOn == 0)
        return RED;
    
    float3 toLightVec = L.position - worldPosition;
    float d = length(toLightVec);
    
    if (d > L.range)
        return float4(0.f, 0.f, 0.f, 0.f);
    if (d < 0.01f)
        return float4(1.f, 1.f, 1.f, 1.f);
    
    toLightVec = toLightVec / d; // normalize
    
    float4 ambient = { 0.f, 0.f, 0.f, 0.f };
    float4 diffuse = { 0.f, 0.f, 0.f, 0.f };
    float4 specular = { 0.f, 0.f, 0.f, 0.f };
    float4 emissive = { 0.f, 0.f, 0.f, 0.f };
   
    // Ambient
    float4 sampledColor = DiffuseMap.Sample(LinearSampler, uv);
    ambient = sampledColor * L.ambient * Material.ambient;
    
    // Diffuse
    float lDotN = dot(toLightVec, normal);
    float3 toEye = normalize(CameraPosition - worldPosition);

    [flatten]
    if (lDotN > 0.f)
    {
        float4 diffuseFactor = L.diffuse * Material.diffuse * lDotN;
        diffuse = sampledColor * diffuseFactor;
        GetSpecular(L.specular, Material.specular, toEye, -toLightVec, normal, specular);
    }
    
    // Attenuate
    float att = 1.0f / dot(L.attenuation, float3(1.0f, d, d * d));
    
    ambient *= (att * 5.f);
    diffuse *= att;
    specular *= att;
    emissive *= att;
    
    float shadowFactor = 1.f;
    if (bShadowMapUsing == 1 && bShadowCubeDataLoaded)
    {
        shadowFactor = ComputePointLightShadowFactor(L.position, worldPosition, 0.1);
    }
    
    return (ambient + diffuse + specular) * shadowFactor;
}

void ComputeNormalMapping(inout float3 worldNormal, float3 worldTangent, float2 uv)
{
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

float4 CalculateLitColor(in MeshOutput input)
{
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
    color.w = 1.f;
    
    return color;
}

#endif /* LIGHT_HLSLI */