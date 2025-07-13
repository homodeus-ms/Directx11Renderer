#ifndef LIGHT_HLSLI
#define LIGHT_HLSLI

#include "Defines.hlsli"
#include "Global.hlsli"

struct LightDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    float3 direction;
    float padding;
};

struct MaterialDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
};

cbuffer LightBuffer : register(BUFFER_NUM_LIGHT)
{
    LightDesc GlobalLight;
}

cbuffer MaterialBuffer : register(BUFFER_NUM_MATERIAL)
{
    MaterialDesc Material;
}


// SRV
Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D SpecularMap : register(t2);



// Compute Light Functions
float4 ComputeAmbient(float3 normal, float2 uv)
{
    float4 ambient = GlobalLight.ambient * Material.ambient;
    return DiffuseMap.Sample(LinearSampler, uv) * ambient;
}

float4 ComputeDiffuse(float3 normal, float2 uv)
{
    float4 diffuse = saturate(dot(-GlobalLight.direction, normal));
    float4 sampledColor = DiffuseMap.Sample(LinearSampler, uv);
    return sampledColor * diffuse * GlobalLight.diffuse * Material.diffuse;
}

float4 ComputeSpecular(float3 normal, float3 worldPosition)
{
    float3 reflection = GlobalLight.direction - (2 * normal * dot(GlobalLight.direction, normal));
    reflection = normalize(reflection);
    
    float3 toEye = normalize(CameraPosition - worldPosition);

    float value = saturate(dot(reflection, toEye));
    float specular = pow(value, 10);

    return GlobalLight.specular * Material.specular * specular;
}

float4 ComputeEmmisive(float3 normal, float3 worldPosition)
{
    float3 toEye = normalize(CameraPosition - worldPosition);

    float value = saturate(dot(toEye, normal));
    float emissive = 1.0f - value;

	// min, max, value
    emissive = smoothstep(0.0f, 1.0f, emissive);
    emissive = pow(emissive, 3);

    return GlobalLight.emissive * Material.emissive * emissive;
}

float4 ComputeLight(float3 normal, float2 uv, float3 worldPosition)
{
    float4 ambient = ComputeAmbient(normal, uv);
    float4 diffuse = ComputeDiffuse(normal, uv);
    float4 specular = ComputeSpecular(normal, worldPosition);
    float4 emmisive = ComputeEmmisive(normal, worldPosition);
   
    return ambient + diffuse + specular + emmisive;
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