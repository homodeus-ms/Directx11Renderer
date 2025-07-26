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
};

enum class EMaterialType : uint32
{
	Default = 0,
	RimLight = 1,
	Toon = 2,
	Monotone = 3,

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
	Vec3 CameraPosition{};
	uint32 bEnvLightUsing = 0;
};

struct TransformDesc
{
	Matrix W = Matrix::Identity;
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
	Color specular = Color(1.f, 1.f, 1.f, 1.f); 
	Color emissive = Color(0.f, 0.f, 0.f, 1.f);
	uint32 bUnLit = 0;
	EMaterialType MaterialType = EMaterialType::Default;
	uint32 pad[2]{};
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