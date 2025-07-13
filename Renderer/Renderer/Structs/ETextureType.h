#pragma once

enum class ETextureType : uint8
{
	Diffuse,
	Normal,
	Specular,
	Height,
	End,
};

enum { TEXTURE_TYPE_COUNT = static_cast<uint8>(ETextureType::End) };