#pragma once
#include "EShaderStage.h"
#include "ETextureType.h"

class IConstantBuffer;

enum class EConstBufferRegisterNumber : uint8
{
    Global = 0,
	Transform,
	Light,
	Material,
	BoneBuffer,
	BoneIndex,
};


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
	float dummy = 0.f;
};

struct TransformDesc
{
	Matrix W = Matrix::Identity;
};

struct LightDesc
{
	Color ambient = Color(1.f, 1.f, 1.f, 1.f);
	Color diffuse = Color(1.f, 1.f, 1.f, 1.f);
	Color specular = Color(1.f, 1.f, 1.f, 1.f);
	Color emissive = Color(0.f, 0.f, 0.f, 0.f);

	Vec3 direction{};
	float padding0 = 0;
};

struct MaterialDesc
{
	Color ambient = Color(1.f, 1.f, 1.f, 1.f);
	Color diffuse = Color(1.f, 1.f, 1.f, 1.f);
	Color specular = Color(1.f, 1.f, 1.f, 1.f);
	Color emissive = Color(0.f, 0.f, 0.f, 1.f);
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