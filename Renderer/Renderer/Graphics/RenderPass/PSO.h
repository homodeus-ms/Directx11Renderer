#pragma once

class Shader;
class VertexShader;
class GeometryShader;
class PixelShader;
class InputLayout;
struct PipelineState;

class PSO
{
public:
	PSO();
	virtual ~PSO();

	void operator=(const PSO& pso);
	void SetBlendFactor(const float blendFactor[4]);

	shared_ptr<VertexShader> _VS;
	shared_ptr<GeometryShader> _GS;
	shared_ptr<PixelShader> _PS;
	shared_ptr<InputLayout> _inputLayout;

	PipelineState* _pipelineState = nullptr;

	float _blendFactor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	uint32 _stencilRef = 0;

	D3D11_PRIMITIVE_TOPOLOGY _topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

