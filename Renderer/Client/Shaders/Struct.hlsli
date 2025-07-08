#include "Defines.hlsli"

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
};

cbuffer TransformData : register(b0)
{
    ROW_MAT matWorld;
    ROW_MAT matView;
    ROW_MAT matProjection;
}