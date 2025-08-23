#pragma once
#include "ShaderParameterTypes.h"

enum class EMatTextureType : uint8
{
	Albedo,
	Normal,
	Height,
	AO,
	Specular,
	MetallicRoughness,
	Roughness,
	Emissive,

	End,
};

enum class EIBLTextureType : uint8
{
	Env = static_cast<uint8>(EMatTextureType::End),
	IBL_Spec,
	IBL_Diff,
	IBL_BRDF,

	End,
};

enum class EFilterTextureType : uint8
{
	Filtered = static_cast<uint8>(EIBLTextureType::End),
	Original,
	LUT,

	End,
};

enum class EShadowTextureType : uint8
{
	ShadowCube = static_cast<uint8>(EFilterTextureType::End),
	Shadow,
	DepthMap = static_cast<uint8>(EShadowTextureType::Shadow) + MAX_SHADOW_MAP_COUNT,

	End,
};

enum 
{ 
	MAT_TEXTURE_TYPE_COUNT = static_cast<uint8>(EMatTextureType::End),
	IBL_TEXTURE_TYPE_COUNT = static_cast<uint8>(EIBLTextureType::End) - static_cast<uint8>(EIBLTextureType::Env),
	FILTER_TEXTURE_TYPE_COUNT = static_cast<uint8>(EFilterTextureType::End) - static_cast<uint8>(EFilterTextureType::Filtered),
	SHADOW_TEXTURE_TYPE_COUNT = static_cast<uint8>(EShadowTextureType::End) - static_cast<uint8>(EShadowTextureType::ShadowCube),
};


