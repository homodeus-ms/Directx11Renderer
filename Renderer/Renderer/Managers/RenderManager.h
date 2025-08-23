#pragma once
#include "Structs/EFilterType.h"

class Actor;
class ShadowMap;
class PostEffect;
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
	void SetShowDepthMap(bool bShow) { _bShowDepthMap = bShow; }
	shared_ptr<PostEffect> GetPostEffect() { return _postEffect; }
	void ChangeShaders(shared_ptr<PSO> pso);
	void SetComputeShader(shared_ptr<class PSO> pso);
	
private:
	void UpdateCommonDatasPerFrame(const vector<shared_ptr<LightActor>>& lightActors);
	void DrawActors(const vector<shared_ptr<Actor>>& actors);
	void DrawStencil(const vector<shared_ptr<Actor>>& stenciledActors, shared_ptr<PSO> pso);
	void DrawNormals(const vector<shared_ptr<Actor>>& actors);
	void DrawMirrorScene(vector<shared_ptr<Actor>>& actors, const vector<shared_ptr<Actor>>& reflectActors);
	shared_ptr<SRVBindingInfo> GetDepthMapSRV(const vector<shared_ptr<Actor>>& actors);
	void DrawDepthMapAndShowForDebug(const vector<shared_ptr<Actor>>& actors);
	void RenderShaderToyDemos();
	void ClearGSShader();

	// CS TEST
	void RenderCS();
	void ComputeShaderBarrier();

	shared_ptr<ShadowMap> _shadowMap;
	shared_ptr<PostEffect> _postEffect;
	shared_ptr<Actor> _cubeMapActor = nullptr;
	shared_ptr<SRVBindingInfo> _depthMapSRVInfo;

	bool _bShowDepthMap = false;
	bool _bWireFrameMode = false;

	shared_ptr<PSO> _normalPSO;
	shared_ptr<PSO> _basicMeshPSO;
	shared_ptr<PSO> _staticMeshPSO;
	shared_ptr<PSO> _cubeMapPSO;
	shared_ptr<PSO> _drawStencilPSO;
	shared_ptr<PSO> _usingStencilPSO;
	shared_ptr<PSO> _drawDebugQuadPSO;
	shared_ptr<PSO> _shaderToyPSO;
	shared_ptr<PSO> _testComputePSO;
};

