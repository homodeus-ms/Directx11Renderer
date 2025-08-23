#pragma once
#include "EShaderStage.h"
#include "ETextureType.h"
#include "LightTypes.h"

class IConstantBuffer;

enum class EConstBufferRegisterNumber : uint8
{
    Global = 0,
	Transform,
	DirectionalLight,
	SpotLight,
	PointLight,
	Material,
	BoneBuffer,
	BoneIndex,
	ShadowData,
	PointShadowData,
	Temporal_Common_Use = 10,
	ForUIDebug = 13,
};

enum class EMaterialType : uint32
{
	Default = 0,
	RimLight = 1,
	Toon = 2,
	Monotone = 3,
	EnvMap = 4,
	PBR = 5,

	END,
};

constexpr uint32 GetEMaterialTypeCount()
{
	return static_cast<uint32>(EMaterialType::END);
}
constexpr string GetEMaterialTypeString(EMaterialType type)
{
	switch (type)
	{
	case EMaterialType::Default: return "Default";
	case EMaterialType::RimLight: return "RimLight";
	case EMaterialType::Toon: return "Toon";
	case EMaterialType::Monotone: return "Monotone";
	default: return "";
	}
}

struct BufferBindingInfo
{
	uint8 slot{};
	shared_ptr<IConstantBuffer> buffer{};
	EShaderStage stage{};
	bool dirty = true;
};

struct SRVBindingInfo
{
	uint8 slot = 0;
	EShaderStage stage{};
	ComPtr<ID3D11ShaderResourceView> srv;
};

struct SamplerBindingInfo
{
	uint8 slot = 0;
	EShaderStage stage{};
	ComPtr<ID3D11SamplerState> sampler;
};

struct GlobalDesc
{
	Matrix V = Matrix::Identity;
	Matrix P = Matrix::Identity;
	Matrix VP = Matrix::Identity;
	Matrix invP = Matrix::Identity;
	Vec3 CameraPosition{};
	uint32 bEnvLightUsing = 0;
};

struct TransformDesc
{
	Matrix W = Matrix::Identity;
	Matrix invW = Matrix::Identity;
};

struct SpotLightBuffer
{
	SpotLightDesc spotLightDescs[MAX_SPOT_LIGHT_COUNT];
	uint32 spotLightCount = 0;
	Vec3 padding;
};

struct PointLightBuffer
{
	PointLightDesc pointLightDescs[MAX_POINT_LIGHT_COUNT];
	uint32 pointLightCount = 0;
	Vec3 padding;
};

struct MaterialDesc
{
	Color ambient = Color(1.f, 1.f, 1.f, 1.f);
	Color diffuse = Color(1.f, 1.f, 1.f, 1.f);
	Color specular = Color(1.f, 1.f, 1.f, 20.f); 
	Color emissive = Color(0.f, 0.f, 0.f, 1.f);
	float metallic = 0.f;
	float roughness = 0.f;
	uint32 bUnLit = 0;
	uint32 bGetIBL = 1;
	EMaterialType MaterialType = EMaterialType::Default;
	uint32 bUseHeightMap = 0;
	uint32 bUseAlbedoMap = 0;
	uint32 bUseNormalMap = 0;
	uint32 bUseMetallicRoughnessMap = 0;
	uint32 bUseAOMap = 0;
	uint32 bUseEmissiveMap = 0;
	float pad{};

	// Emissive는 따로 셋팅
	void UsePBR()
	{
		bUseHeightMap = 1;
		bUseAlbedoMap = 1;
		bUseNormalMap = 1;
		bUseMetallicRoughnessMap = 1;
		bUseAOMap = 1;
		metallic = 0.4f;
		roughness = 0.5f;
		MaterialType = EMaterialType::PBR;
	}
};


#define MAX_BONE_COUNT 50

struct BoneBuffer
{
	Matrix transforms[MAX_BONE_COUNT];
};

struct BoneIndex
{
	uint32 boneIndex;
	float padding[3];
};

struct ShadowDataDesc
{
	Matrix lightVP[MAX_SHADOW_MAP_COUNT];
};

struct PointShadowDataDesc
{
	Matrix lightVP[6]{};
	Vec3 lightPosition{};
	float pad;
};

struct CurrentLightVPIndex
{
	uint32 index = 0;
	float pad[3];
};

struct FilterData
{
	float dx;
	float dy;
	float brightnessThreshold;
	float strength = 0.4f;
	Vec4 options;
};

struct ForUIDebugDesc
{
	float heightScale = 1.f;
	float pad[3];
};

struct PostEffectData
{
	Vec3 fogColor = { 0.8f, 0.55f, 0.1f };
	int32 mode = 0;
	float depthScale = 0.01f;
	float fogStrength = 1.f;
	float pad[2]{};
};

struct ShaderToyData
{
	Vec3 iResolution = Vec3(VIEW_X, VIEW_Y, 1.f); // viewport resolution (in pixels)
	float iTime = 30.f; // shader playback time (in seconds)

	float iTimeDelta{}; // render time (in seconds)
	float iFrameRate{}; // shader frame rate
	int32 iFrame = 0;
	float _padding{};

	//Vec4 iChannelTime{}; // channel playback time (in seconds)
	//Vec3 iChannelResolution[4]{}; // channel resolution (in pixels)
	//float _padding2{};
	//Vec4 iMouse{}; // mouse pixel coords. xy: current (if MLB down), zw: click
	//Vec4 iDate{}; // (year, month, day, time in seconds)
};