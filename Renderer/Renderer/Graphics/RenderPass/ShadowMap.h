#pragma once
#include "ShadowMapResources.h"

class Actor;

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	void Construct();
	void CreateShadowMap(vector<shared_ptr<Actor>>& actors);
	const ShadowMapResources& GetShadowMapResources() { return _resources; }

private:
	void CreateShadowTexture();
	void CreateShadowDSV();
	void CreateShadowSRV();
	void SetShadowViewport();
	void CreateShadowMapResources();

	enum { SHADOW_MAP_SIZE = 2048 };

	ComPtr<ID3D11Texture2D> _shadowTexture;
	ComPtr<ID3D11DepthStencilView> _shadowDSV;
	ComPtr<ID3D11ShaderResourceView> _shadowSRV;
	D3D11_VIEWPORT _shadowViewport{};

	// Render Resources
	const wstring SHADER_NAME = L"GetDepthShader.hlsl";
	ShadowMapResources _resources;
	shared_ptr<class ShaderInfo> _shaderInfo;
};

