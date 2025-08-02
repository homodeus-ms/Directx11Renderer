#pragma once
#include "Texture.h"

class Texture3D : public Texture
{
	using Super = Texture;

public:
	Texture3D();
	virtual ~Texture3D();

	void CreateTexture(uint32 size, const vector<Vec3>& data);

protected:
	ComPtr<ID3D11Texture3D> _texture;
	
};

