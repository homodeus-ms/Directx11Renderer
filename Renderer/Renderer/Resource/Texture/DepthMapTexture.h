#pragma once
#include "Texture.h"

class DepthMapTexture : public Texture
{
	using Super = Texture;
public:
	DepthMapTexture();
	virtual ~DepthMapTexture();

	ComPtr<ID3D11Texture2D> GetTexture() const { return _texture; }
	ComPtr<ID3D11DepthStencilView> GetDSV() const { return _DSV; }
	
	virtual void CreateTexture(EShadowTextureType type);

protected:
	ComPtr<ID3D11Texture2D> _texture;
	ComPtr<ID3D11DepthStencilView> _DSV;
};

