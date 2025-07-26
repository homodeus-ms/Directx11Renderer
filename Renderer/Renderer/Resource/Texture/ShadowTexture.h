#pragma once
#include "Texture.h"

class ShadowTexture : public Texture
{
	using Super = Texture;
public:
	ShadowTexture();
	virtual ~ShadowTexture();

	ComPtr<ID3D11Texture2D> GetTexture() const { return _texture; }
	ID3D11Texture2D* const* GetTextureAddress() const { return _texture.GetAddressOf(); }

	ComPtr<ID3D11DepthStencilView> GetDSV() const { return _DSV; }
	ID3D11DepthStencilView* const* GetDSVAddress() const { return _DSV.GetAddressOf(); }

	virtual void CreateTexture();

protected:
	ComPtr<ID3D11Texture2D> _texture;
	ComPtr<ID3D11DepthStencilView> _DSV;
};

