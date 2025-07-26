#pragma once
#include "ShadowTexture.h"

class ShadowCubeTexture : public ShadowTexture
{
	using Super = Texture;

public:
	ShadowCubeTexture();
	virtual ~ShadowCubeTexture();

	const array<ComPtr<ID3D11RenderTargetView>, 6>& GetRTVs() const { return _RTVs; }
	ComPtr<ID3D11RenderTargetView> GetRTV() const { return _RTV; }
	virtual void CreateTexture() override;

private:
	array<ComPtr<ID3D11RenderTargetView>, 6> _RTVs;
	ComPtr<ID3D11RenderTargetView> _RTV;
};

