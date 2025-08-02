#pragma once
#include "ShaderParameterTypes.h"

enum class ETextureType : uint8
{
	Diffuse,
	Normal,
	Specular,
	IBL_Spec,
	IBL_Diff,
	Shadow,
	Filter,
	LUT,

	End,
};

enum { TEXTURE_TYPE_COUNT = static_cast<uint8>(ETextureType::End) };

enum
{
	SHADOW_CUBE_MAP_SLOT_NUM = static_cast<uint8>(ETextureType::Shadow) + MAX_SHADOW_MAP_COUNT,
	FILTER_TARGET_SLOT_NUM = SHADOW_CUBE_MAP_SLOT_NUM + 1,
	FILTER_ORIGINAL_SLOT_NUM = FILTER_TARGET_SLOT_NUM + 1,
	LUT_SLOT_NUM = FILTER_ORIGINAL_SLOT_NUM + 1,
};
