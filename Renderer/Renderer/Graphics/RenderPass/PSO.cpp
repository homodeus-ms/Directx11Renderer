#include "pch.h"
#include "PSO.h"
#include "../Shader/Shader.h"
#include "../Shader/VertexShader.h"
#include "../Shader/GeometryShader.h"
#include "../Shader/PixelShader.h"

PSO::PSO()
{
	
}

PSO::~PSO()
{
}

void PSO::operator=(const PSO& pso)
{
	_VS = pso._VS;
	_GS = pso._GS;
	_PS = pso._PS;
	_inputLayout = pso._inputLayout;
	_topology = pso._topology;
	_pipelineState = pso._pipelineState;
	for (int32 i = 0; i < 4; ++i)
		_blendFactor[i] = pso._blendFactor[i];
	_stencilRef = pso._stencilRef;
}

void PSO::SetBlendFactor(const float blendFactor[4])
{
	memcpy(_blendFactor, blendFactor, sizeof(float) * 4);
}
