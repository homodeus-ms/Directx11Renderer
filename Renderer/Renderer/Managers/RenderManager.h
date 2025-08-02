#pragma once
#include "Structs/EFilterType.h"

class Actor;
class ShadowMap;
class LightActor;
struct PipelineState;

#define RENDER_MANAGER GET_SINGLE(RenderManager)

class RenderManager
{
	DECLARE_SINGLE(RenderManager);

public:
	void BeginPlay();

	void SetShadowMap(const shared_ptr<ShadowMap>& shadowMap) { _shadowMap = shadowMap; }
	void Render(vector<shared_ptr<Actor>>& actors);
	

private:
	void UpdateCommonDatas(const vector<shared_ptr<LightActor>>& lightActors);

	shared_ptr<ShadowMap> _shadowMap;

};

