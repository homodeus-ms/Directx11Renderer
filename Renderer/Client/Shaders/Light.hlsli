#ifndef LIGHT_HLSLI
#define LIGHT_HLSLI

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

cbuffer LightBuffer : register(b2)
{
    LightDesc GlobalLight;
}

cbuffer MaterialBuffer : register(b3)
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
    float4 diffuse = dot(-GlobalLight.direction, normal);
    float4 sampledColor = DiffuseMap.Sample(LinearSampler, uv);
    return sampledColor * diffuse * GlobalLight.diffuse * Material.diffuse;
}

float4 ComputeSpecular(float3 normal, float3 worldPosition)
{
    float3 reflection = GlobalLight.direction - (2 * normal * dot(GlobalLight.direction, normal));
    reflection = normalize(reflection);

    float3 cameraPosition = CameraPosition();
    float3 toEye = normalize(cameraPosition - worldPosition);

    float value = saturate(dot(reflection, toEye));
    float specular = pow(value, 10);

    return GlobalLight.specular * Material.specular * specular;
}

float4 ComputeEmmisive(float3 normal, float3 worldPosition)
{
    float3 cameraPosition = CameraPosition();
    float3 toEye = normalize(cameraPosition - worldPosition);

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

    return ambient + diffuse + specular;
    //return ambient + diffuse + specular + emmisive;
    //return specular;
}



#endif /* LIGHT_HLSLI */