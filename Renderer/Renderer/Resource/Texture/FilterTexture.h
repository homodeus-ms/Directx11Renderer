#pragma once
#include "Texture.h"
class FilterTexture : public Texture
{
	using Super = Texture;

public:
	FilterTexture();
	virtual ~FilterTexture();

	ComPtr<ID3D11Texture2D> GetTexture() const { return _texture; }
	ComPtr<ID3D11RenderTargetView> GetRTV() const { return _RTV; }

	void CreateTexture(uint32 w, uint32 h);

private:
	ComPtr<ID3D11Texture2D> _texture;
	ComPtr<ID3D11RenderTargetView> _RTV;
};

