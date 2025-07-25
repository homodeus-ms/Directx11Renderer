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
    float4 worldPosition : POSITION;
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
    uint bUnLit;
    uint MaterialType;
    uint pad[2];
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
    float3 CameraPosition;
    uint bEnvLightUsing;
};

cbuffer TransformBuffer : register(CBUFFER_NUM_TRANSFORM)
{
    ROW_MAT W;
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
    ROW_MAT lightViews[6];
    ROW_MAT lightVP[6];
    uint currUseCount;
    uint bShadowMapUsing;
    uint bShadowCubeDataLoaded;
    float shadowPad;
}


// SRV
Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D SpecularMap : register(t2);
TextureCube textureCube : register(t3);
Texture2D ShadowMaps[MAX_ACTIVE_SHADOW_LIGHT] : register(SHADOW_MAP_REG_NUM);
TextureCube ShadowCubeMap : register(SHADOW_CUBE_MAP_REG_NUM);


// SamplerState
//SamplerState LinearSampler
//{
//    Filter = MIN_MAG_MIP_LINEAR;
//    AddressU = Wrap;
//    AddressV = Wrap;
//};
SamplerState LinearSampler : register(s0);

SamplerState PointSampler
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};

// RasterizerState
RasterizerState FillModeWireFrame
{
    FillMode = WireFrame;
};

// Function
void Normalize(inout float3 normal, inout float3 tangent)
{
    normal = normalize(normal);
    tangent = normalize(tangent);
}


#endif /* STRUCT_HLSLI */