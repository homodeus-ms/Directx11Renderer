#ifndef LIGHT_HLSLI
#define LIGHT_HLSLI

#include "Defines.hlsli"
#include "Global.hlsli"

#define SPECULAR_INTENSITY (30)
#define EMISSIVE_INTENSITY (10)

struct DirectionalLightDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    
    float3 direction;
    uint isOn;
};

struct SpotLightDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    
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
    
    float3 position;
    float range;
    float3 attenuation; // constant, linear, quadratic
    uint isOn;
};

cbuffer DirectionalLightBuffer : register(BUFFER_NUM_DIRECTIONAL_LIGHT)
{
    DirectionalLightDesc GlobalLight;
}

cbuffer SpotLightBuffer : register(BUFFER_NUM_SPOT_LIGHT)
{
    SpotLightDesc SpotLights[MAX_SPOT_LIGHT_COUNT];
    uint SpotlightCount;
    float3 spotLight_padding;
}

cbuffer PointLightBuffer : register(BUFFER_NUM_POINT_LIGHT)
{
    PointLightDesc PointLights[MAX_POINT_LIGHT_COUNT];
    uint PointlightCount;
    float3 pointLight_padding;
}

// SRV
Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D SpecularMap : register(t2);

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

void GetEmissive(float4 lightE, float4 matE, float3 toEye, float3 normal, out float4 emissive)
{
    float eyeDotNormal = saturate(dot(toEye, normal));
    float factor = 1.f - eyeDotNormal;
    
    factor = smoothstep(0.0f, 1.0f, factor);
    factor = pow(factor, EMISSIVE_INTENSITY);
    emissive = lightE * matE * factor;
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
    
    // emissive
    GetEmissive(GlobalLight.emissive, Material.emissive, toEye, normal, emissive);
    
    return ambient + diffuse + specular + emissive;
}

float4 ComputeSpotLight(SpotLightDesc L, float3 normal, float2 uv, float3 worldPosition)
{
    if (L.isOn == 0)
        return float4(0.f, 0.f, 0.f, 0.f);
    
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
    
    // emissive
    GetEmissive(GlobalLight.emissive, Material.emissive, toEye, normal, emissive);
    
    // SpotPower
    float spot = pow(max(dot(-toLightVec, L.direction), 0.0f), L.spotPower);
    float att = spot / dot(L.attenuation, float3(1.0f, d, d * d));
    
    ambient *= spot;
    diffuse *= att;
    specular *= att;
    
    return ambient + diffuse + specular + emissive;
}

float4 ComputePointLight(PointLightDesc L, float3 normal, float2 uv, float3 worldPosition)
{
    if (L.isOn == 0)
        return float4(0.f, 0.f, 0.f, 0.f);
    
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
    
    // emissive
    GetEmissive(GlobalLight.emissive, Material.emissive, toEye, normal, emissive);
    
    // Attenuate
    float att = 1.0f / dot(L.attenuation, float3(1.0f, d, d * d));
    diffuse *= att;
    specular *= att;
    emissive *= att;
    
    return ambient + diffuse + specular + emissive;
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

#endif /* LIGHT_HLSLI */