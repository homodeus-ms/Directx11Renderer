#pragma once
#include "ShadowMapResources.h"
#include "Structs/LightTypes.h"

class Actor;
class LightActor;
struct SRVBindingInfo;
class ShadowTexture;
class ShadowCubeTexture;

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	void Construct();
	void CreateShadowMap(vector<shared_ptr<Actor>>& actors, const vector<shared_ptr<LightActor>>& lights);
	const ShadowMapResources& GetShadowMapResources() { return _resources; }
	
private:
	void CreateShadowTexture();
	void SetShadowViewport();
	void CreateShadowMapResources();
	void DrawShadowMap(const vector<shared_ptr<Actor>>& actors, int32 drawShadowMapCount);
	void DrawShadowCubeMap(shared_ptr<LightActor> light, const vector<shared_ptr<Actor>>& actors);

	//array<ShadowTexture*, MAX_SHADOW_MAP_COUNT> _shadowTextures{};
	ShadowTexture* _shadowTexture{};
	ShadowCubeTexture* _shadowCubeTexture{};
	D3D11_VIEWPORT _shadowViewport{};

	uint32 _currIndex = 0;
	const float _cubeMapClearColor[4] = {1.f, 0.f, 0.f, 1.f};
	const wstring SHADER_NAME = L"GetDepthShader.hlsl";
	const wstring POINT_LIGHT_SHADER_NAME = L"GetDepthShaderForPoint.hlsl";

	ShadowMapResources _resources{};
	shared_ptr<struct ShaderInfo> _defaultShaderInfo{};
	shared_ptr<struct ShaderInfo> _pointLightShaderInfo{};

};

