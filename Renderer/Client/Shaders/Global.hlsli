#ifndef STRUCT_HLSLI
#define STRUCT_HLSLI

// Macros
#define ROW_MAT row_major matrix

// Input
struct VertexColorInput
{
    float4 Position : POSITION;
    float4 Color : COLOR;
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
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
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

cbuffer GlobalBuffer : register(b0)
{
    ROW_MAT View;
    ROW_MAT Projection;
    ROW_MAT VP;
};

cbuffer TransformBuffer : register(b1)
{
    ROW_MAT World;
};

// SamplerState
SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

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

// Functions
float3 CameraPosition()
{
    return -View._41_42_43;
}

#endif /* STRUCT_HLSLI */