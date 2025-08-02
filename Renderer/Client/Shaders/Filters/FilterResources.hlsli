#ifndef FILTER_RESOURCES_HLSLI
#define FILTER_RESOURCES_HLSLI

#include "C:\Users\seekc\OneDrive\Documents\Renderer\Renderer\Client\Shaders\Defines.hlsli"
//#include "C:\Users\seekc\OneDrive\Documents\Renderer\Renderer\Client\Shaders\Global.hlsli"

struct VertexUVInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexUVOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
};

SamplerState FilterSampler : register(s0);

cbuffer FilterDataBuffer : register(b0)
{
    float dx;
    float dy;
    float brightnessThreshold;
    float strength;
    float4 options;
}

Texture2D FilterTargetTexture : register(FILTER_TARGET_REG_NUM);
Texture2D OriginalTexture : register(FILTER_ORIGINAL_REG_NUM);
Texture3D LUTTexture : register(LUT_REG_NUM);

static const float GAUSSIAN_WEIGHTS[5] = { 0.0545, 0.2442, 0.4026, 0.2442, 0.0545 };

#endif /* FILTER_RESOURCES_HLSLI */