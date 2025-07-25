#pragma once

enum class ETextureType : uint8
{
	Diffuse,
	Normal,
	Specular,
	
	End,
};

enum { TEXTURE_TYPE_COUNT = static_cast<uint8>(ETextureType::End) };

enum
{
	IBL_LIGHT_SLOT_NUM = TEXTURE_TYPE_COUNT,
	SHADOW_SLOT_NUM = IBL_LIGHT_SLOT_NUM + 1,
};
