#pragma once
#include "Texture.h"
class RenderTexture : public Texture
{
	using Super = Texture;

public:
	RenderTexture();
	virtual ~RenderTexture();

	ComPtr<ID3D11Texture2D> GetTexture() const { return _texture; }
	ComPtr<ID3D11RenderTargetView> GetRTV() const { return _RTV; }

	void CreateTexture(uint32 w, uint32 h, uint8 srvSlotNum);

private:
	ComPtr<ID3D11Texture2D> _texture;
	ComPtr<ID3D11RenderTargetView> _RTV;
};

