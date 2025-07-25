#pragma once
#include "ShadowMapResources.h"
#include "Structs/LightTypes.h"

class Actor;
class LightActor;
struct SRVBindingInfo;

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	void Construct();
	void CreateShadowMap(vector<shared_ptr<Actor>>& actors, const vector<shared_ptr<LightActor>>& lights);
	const ShadowMapResources& GetShadowMapResources() { return _resources; }
	ComPtr<ID3D11ShaderResourceView> GetShadowCubeSRV() { return _shadowCubeSRV; }

private:
	void CreateShadowTexture();
	void CreateShadowDSV();
	void CreateShadowSRV();
	void CreateShadowCubeRTV();
	void SetShadowViewport();
	void CreateShadowMapResources();
	void DrawShadowMap(shared_ptr<LightActor> light, const vector<shared_ptr<Actor>>& actors, int32 index);
	void DrawShadowCubeMap(shared_ptr<LightActor> light, const vector<shared_ptr<Actor>>& actors);

	// Shadow Map Render Resources
	array<ComPtr<ID3D11Texture2D>, MAX_LIGHTS_COUNT> _shadowTextures;
	array<ComPtr<ID3D11DepthStencilView>, MAX_LIGHTS_COUNT> _shadowDSVs;
	array<ComPtr<ID3D11ShaderResourceView>, MAX_LIGHTS_COUNT> _shadowSRVs;
	array<shared_ptr<SRVBindingInfo>, MAX_LIGHTS_COUNT> _shadowSRVBindingInfos;

	// For CubeTexture
	ComPtr<ID3D11Texture2D> _shadowCubeTexture;
	array<ComPtr<ID3D11RenderTargetView>, 6> _shadowCubeRTVs;
	//array<ComPtr<ID3D11DepthStencilView>, 6> _shadowCubeDSVs;    // TEMP
	ComPtr<ID3D11ShaderResourceView> _shadowCubeSRV;
	shared_ptr<SRVBindingInfo> _shadowCubeSRVBindingInfo;

	D3D11_VIEWPORT _shadowViewport{};

	uint32 _currIndex = 0;
	
	const wstring SHADER_NAME = L"GetDepthShader.hlsl";
	const wstring POINT_LIGHT_SHADER_NAME = L"GetDepthShaderForPoint.hlsl";

	ShadowMapResources _resources{};
	shared_ptr<struct ShaderInfo> _defualtShaderInfo;
	shared_ptr<struct ShaderInfo> _pointLightShaderInfo;

};

