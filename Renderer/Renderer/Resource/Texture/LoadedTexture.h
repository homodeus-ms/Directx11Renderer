#pragma once
#include "Texture.h"

class LoadedTexture : public Texture
{
	using Super = Texture;

public:
	LoadedTexture();
	virtual ~LoadedTexture();
	
	virtual void Load(const wstring& path) override;

	Vec2 GetSize() const { return _size; }

	const DirectX::ScratchImage& GetInfo() const { return _img; }

private:
	
	Vec2 _size = { 0.f, 0.f };
	DirectX::ScratchImage _img = {};
};

