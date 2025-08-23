#ifndef STRUCT_HLSLI
#define STRUCT_HLSLI

#include "Defines.hlsli"

// Input
struct VertexPosOnlyInput
{
    float4 position : POSITION;
};

struct VertexColorInput
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VertexUVInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct VertexTangentInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

// Output
struct VertexClipPosOnlyOutput
{
    float4 clipPosition : SV_POSITION;
};

struct VertexPosOnlyOutput
{
    float4 worldPosition : SV_Position;
};

struct VertexColorOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


struct VertexUVOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
};

struct VertexOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct MeshOutput
{
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION1;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct MaterialDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    float metallic;
    float roughness;
    uint bUnLit;
    uint bGetIBL;
    uint MaterialType;
    uint bUseHeightMap;
    uint bUseAlbedoMap;
    uint bUseNormalMap;
    uint bUseMetallicRoughnessMap;
    uint bUseAOMap;
    uint bUseEmissiveMap;
    float materialPad;
};

cbuffer MaterialBuffer : register(CBUFFER_NUM_MATERIAL)
{
    MaterialDesc Material;
}

cbuffer GlobalBuffer : register(CBUFFER_NUM_GLOBAL)
{
    ROW_MAT V;
    ROW_MAT P;
    ROW_MAT VP;
    ROW_MAT invP;
    float3 CameraPosition;
    uint bEnvLightUsing;
};

cbuffer TransformBuffer : register(CBUFFER_NUM_TRANSFORM)
{
    ROW_MAT W;
    ROW_MAT invW;
};

#define MAX_BONE_COUNT 50

cbuffer BoneBuffer : register(CBUFFER_NUM_BONE)
{
    ROW_MAT BoneTransforms[MAX_BONE_COUNT];
}

cbuffer BoneIndex : register(CBUFFER_NUM_BONEINDEX)
{
    uint BoneIndex;
    float3 boneIndexPadding;
}

cbuffer ShadowData : register(CBUFFER_NUM_SHADOW)
{
    ROW_MAT g_LightVP[MAX_ACTIVE_SHADOW_LIGHT];
}

cbuffer PointShadowData : register(CBUFFER_NUM_POINT_SHADOW)
{
    ROW_MAT g_PointLightVPs[6];
    float3 g_ShadowedPointLightPosition;
    float pointShadowDataPad;
}
cbuffer ForUIDebug : register(CBUFFER_NUM_FOR_UI_DEBUG)
{
    float g_heightScale;
    float3 forUIDebugPad;
}

// SRV
Texture2D AlbedoMap : register(REG_ALBEDO);
Texture2D NormalMap : register(REG_NORMAL);
Texture2D HeightMap : register(REG_HEIGHT);
Texture2D AOMap : register(REG_AO);
Texture2D SpecularMap : register(REG_SPECULAR);
Texture2D MetallicRoughnessMap : register(REG_METALLIC_ROUGHNESS);
Texture2D EmissiveMap : register(REG_EMISSIVE);

TextureCube EnvMap : register(REG_ENV);
TextureCube IBLSpec : register(REG_IBL_SPEC);
TextureCube IBLDiff : register(REG_IBL_DIFF);
Texture2D BRDFMap : register(REG_BRDF);

TextureCube ShadowCubeMap : register(REG_SHADOW_CUBE);
Texture2D ShadowMaps[MAX_ACTIVE_SHADOW_LIGHT] : register(REG_SHADOW);
Texture2D DepthMap : register(REG_DEPTH_MAP);

// SamplerState
SamplerState LinearSampler : register(s0);
SamplerState ClampSampler : register(s1);
SamplerComparisonState ComparisionSampler : register(s2);

SamplerState PointSampler
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};

// Function
void Normalize(inout float3 normal, inout float3 tangent)
{
    normal = normalize(normal);
    tangent = normalize(tangent);
}

float GetMipLevel(float3 worldPosition)
{
    const float dist = length(CameraPosition - worldPosition);
    const float distMin = 30.f;
    const float distMax = 80.f;
    const float lod = 10.f * saturate((dist - distMin) / (distMax - distMin));
    return lod;
}


#endif /* STRUCT_HLSLI */