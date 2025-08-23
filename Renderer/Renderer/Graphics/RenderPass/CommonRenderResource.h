#pragma once
#include "PSO.h"

class CommonRenderResource
{
	DECLARE_SINGLE(CommonRenderResource);

public:
	void Initialize();

	shared_ptr<PSO> _normalPSO;
	shared_ptr<PSO> _basicMeshPSO;
	shared_ptr<PSO> _staticMeshPSO;
	shared_ptr<PSO> _cubeMapPSO;
	shared_ptr<PSO> _getDepthMapPSO;
	shared_ptr<PSO> _showDepthMapPSO;

	shared_ptr<PSO> _shadowPSO;
	shared_ptr<PSO> _shadowPointLightPSO;
	shared_ptr<PSO> _drawStencilPSO;
	shared_ptr<PSO> _usingStencilPSO;

	shared_ptr<PSO> _postEffectPSO;
	shared_ptr<PSO> _shaderToyPSO;

	shared_ptr<PSO> _testComputePSO;

private:
	void InitProperties();
	shared_ptr<InputLayout> _vertexTangentIL;
	shared_ptr<PipelineState> _defaultState;
};

