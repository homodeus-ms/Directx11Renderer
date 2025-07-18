#ifndef STRUCT_HLSLI
#define STRUCT_HLSLI

#include "Defines.hlsli"

// Input
struct VertexColorInput
{
    float4 position : POSITION;
    float4 color : COLOR;
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
struct VertexColorOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
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
};

cbuffer MaterialBuffer : register(BUFFER_NUM_MATERIAL)
{
    MaterialDesc Material;
}

cbuffer GlobalBuffer : register(BUFFER_NUM_GLOBAL)
{
    ROW_MAT V;
    ROW_MAT P;
    ROW_MAT VP;
    float3 CameraPosition;
    float globalBufferPadding;
};

cbuffer TransformBuffer : register(BUFFER_NUM_TRANSFORM)
{
    ROW_MAT W;
};

#define MAX_BONE_COUNT 50

cbuffer BoneBuffer : register(BUFFER_NUM_BONE)
{
    ROW_MAT BoneTransforms[MAX_BONE_COUNT];
}

cbuffer BoneIndex : register(BUFFER_NUM_BONEINDEX)
{
    uint BoneIndex;
    float3 boneIndexPadding;
}

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