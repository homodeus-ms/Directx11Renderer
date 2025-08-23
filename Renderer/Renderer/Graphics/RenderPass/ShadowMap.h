#pragma once
#include "ShadowMapResources.h"
#include "Structs/LightTypes.h"

class Actor;
class LightActor;
struct SRVBindingInfo;
class DepthMapTexture;
class CubeDepthMapTexture;
class PSO;

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	void Construct();
	shared_ptr<SRVBindingInfo> DrawDepthMap(const vector<shared_ptr<Actor>>& actors);
	void CreateAndDrawShadowMap(vector<shared_ptr<Actor>>& actors, const vector<shared_ptr<LightActor>>& lights);
	
private:
	void CreateShadowTexture();
	void SetShadowViewport();

	void DrawShadowMap(const vector<shared_ptr<Actor>>& actors, int32 index);
	void DrawShadowCubeMap(shared_ptr<LightActor> light, const vector<shared_ptr<Actor>>& actors);

	DepthMapTexture* _depthMapTexture;
	array<DepthMapTexture*, MAX_SHADOW_MAP_COUNT> _shadowTextures{};
	CubeDepthMapTexture* _shadowCubeTexture{};
	D3D11_VIEWPORT _shadowViewport{};

	shared_ptr<PSO> _depthMapPSO;
	shared_ptr<PSO> _shadowPSO;
	shared_ptr<PSO> _shadowPointLightPSO;

	uint32 _currIndex = 0;
	const float _cubeMapClearColor[4] = {1.f, 0.f, 0.f, 1.f};


};

