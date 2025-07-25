#ifndef DEFINES_HLSLI
#define DEFINES_HLSLI


#define ROW_MAT row_major matrix
#define BLACK float4(0.f, 0.f, 0.f, 1.f)
#define WHITE float4(1.f, 1.f, 1.f, 1.f)
#define RED float4(1.f, 0.f, 0.f, 1.f)
#define GREEN float4(0.f, 1.f, 0.f, 1.f)
#define BLUE float4(0.f, 0.f, 1.f, 1.f)
#define YELLOW float4(1.f, 1.f, 0.f, 1.f)

#define DEFAULT_LIM_LIGHT_COLOR float4(1.f, 0.7f, 0.f, 1.f)
// ConstantBuffer Register Num
#define CBUFFER_NUM_GLOBAL b0
#define CBUFFER_NUM_TRANSFORM b1
#define CBUFFER_NUM_DIRECTIONAL_LIGHT b2
#define CBUFFER_NUM_SPOT_LIGHT b3
#define CBUFFER_NUM_POINT_LIGHT b4
#define CBUFFER_NUM_MATERIAL b5
#define CBUFFER_NUM_BONE b6
#define CBUFFER_NUM_BONEINDEX b7
#define CBUFFER_NUM_SHADOW b8

#define MAX_DIRECTIONAL_LIGHT_COUNT 1
#define MAX_SPOT_LIGHT_COUNT 3
#define MAX_POINT_LIGHT_COUNT 3
#define MAX_LIGHTS 7

#define MATERIAL_TYPE_DEFAULT 0
#define MATERIAL_TYPE_LIM_LIGHT 1
#define MATERIAL_TYPE_TOON 2
#define MATERIAL_TYPE_MONOTONE 3

#define MAX_ACTIVE_SHADOW_LIGHT 4
#define SHADOW_MAP_REG_NUM t4
#define SHADOW_CUBE_MAP_REG_NUM t8

#endif /* DEFINES_HLSLI */