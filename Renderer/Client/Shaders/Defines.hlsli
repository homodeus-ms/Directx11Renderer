#ifndef DEFINES_HLSLI
#define DEFINES_HLSLI

#define PI 3.141592f
#define EPSILON 1e-5

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
#define CBUFFER_NUM_POINT_SHADOW b9
#define CBUFFER_NUM_FOR_UI_DEBUG b13

#define MAX_DIRECTIONAL_LIGHT_COUNT 1
#define MAX_SPOT_LIGHT_COUNT 3
#define MAX_POINT_LIGHT_COUNT 3
#define MAX_LIGHTS 7

#define MATERIAL_TYPE_DEFAULT 0
#define MATERIAL_TYPE_LIM_LIGHT 1
#define MATERIAL_TYPE_TOON 2
#define MATERIAL_TYPE_MONOTONE 3
#define MATERIAL_TYPE_EnvMap 4
#define MATERIAL_TYPE_PBR 5

#define MAX_ACTIVE_SHADOW_LIGHT 4

// Texture Reg Num
#define REG_ALBEDO t0
#define REG_NORMAL t1
#define REG_HEIGHT t2
#define REG_AO t3
#define REG_SPECULAR t4
#define REG_METALLIC_ROUGHNESS t5
#define REG_ROUGHNESS t6
#define REG_EMISSIVE t7

#define REG_ENV t8
#define REG_IBL_SPEC t9
#define REG_IBL_DIFF t10
#define REG_BRDF t11

#define REG_FILTER_COMBINE_FILTERED t12
#define REG_FILTER_COMBINE_ORIGIN t13
#define REG_LUT t14

#define REG_SHADOW_CUBE t15
#define REG_SHADOW t16   // t16, t17, t18, t19  (Max Shadow : 4 )


#endif /* DEFINES_HLSLI */