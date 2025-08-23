#pragma once
#include "DepthMapTexture.h"

class CubeDepthMapTexture : public DepthMapTexture
{
	using Super = Texture;

public:
	CubeDepthMapTexture();
	virtual ~CubeDepthMapTexture();

	const array<ComPtr<ID3D11RenderTargetView>, 6>& GetRTVs() const { return _RTVs; }
	ComPtr<ID3D11RenderTargetView> GetRTV() const { return _RTV; }
	virtual void CreateTexture(EShadowTextureType type) override;

private:
	array<ComPtr<ID3D11RenderTargetView>, 6> _RTVs;
	ComPtr<ID3D11RenderTargetView> _RTV;
};

