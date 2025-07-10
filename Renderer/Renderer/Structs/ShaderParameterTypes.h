#pragma once
#include "EShaderStage.h"

class IConstantBuffer;

//enum class EConstBufferType : uint8
//{
//	Global = 0,
//	Transform,
//	Light,
//	Material,
//	End,
//};


struct BufferBindingInfo
{
	uint8 slot{};
	shared_ptr<IConstantBuffer> buffer{};
	EShaderStage stage{};
	bool dirty = true;
};

enum class ETextureType : uint8
{
	Diffuse = 0,
	Normal,
	Specular,
	End,
};

enum { TEXTURE_TYPE_COUNT = static_cast<uint8>(ETextureType::End) };

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
