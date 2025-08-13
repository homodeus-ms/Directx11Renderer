#pragma once
#include "Structs/EFilterType.h"

class Actor;
class ShadowMap;
class LightActor;
struct PipelineState;
class PSO;

#define RENDER_MANAGER GET_SINGLE(RenderManager)

class RenderManager
{
	DECLARE_SINGLE(RenderManager);

public:
	void BeginPlay();

	void SetShadowMap(const shared_ptr<ShadowMap>& shadowMap) { _shadowMap = shadowMap; }
	void Render(vector<shared_ptr<Actor>>& actors);
	void SetCubeMapActor(shared_ptr<Actor> cubeMapActor);
	void SetWireFrameMode(bool bModeOn) { _bWireFrameMode = bModeOn; }
	void SetPipelineState(shared_ptr<class PSO> pso);

private:
	void UpdateCommonDatasPerFrame(const vector<shared_ptr<LightActor>>& lightActors);
	void ChangeShaders(shared_ptr<PSO> pso);
	void DrawActors(const vector<shared_ptr<Actor>>& actors);
	void DrawStencil(const vector<shared_ptr<Actor>>& stenciledActors, shared_ptr<PSO> pso);
	void DrawMirrorScene(vector<shared_ptr<Actor>>& actors, const vector<shared_ptr<Actor>>& reflectActors);
	void ClearGSShader();

	shared_ptr<ShadowMap> _shadowMap;
	shared_ptr<Actor> _cubeMapActor = nullptr;

	bool _bWireFrameMode = false;

	shared_ptr<PSO> _normalPSO;
	shared_ptr<PSO> _basicMeshPSO;
	shared_ptr<PSO> _staticMeshPSO;
	shared_ptr<PSO> _cubeMapPSO;
	shared_ptr<PSO> _drawStencilPSO;
	shared_ptr<PSO> _usingStencilPSO;
};

