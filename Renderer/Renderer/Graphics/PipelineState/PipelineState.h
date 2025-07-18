#pragma once

class SamplerState;

class PipelineState
{
public:
	PipelineState();
	~PipelineState();

	static shared_ptr<PipelineState> GetDefaultState();

	float* GetBlendFactor() { return &_blendFactor; }
	uint32 GetSampleMask() { return _sampleMask; }

	D3D11_PRIMITIVE_TOPOLOGY GetTopology() const { return _topology; }
	ComPtr<ID3D11RasterizerState> GetRsState() const { return _rsState; }
	ComPtr<ID3D11BlendState> GetBlendState() const { return _blendState; }
	ComPtr<ID3D11SamplerState> GetSamplerState() const;
	ComPtr<ID3D11DepthStencilState> GetDsState() const { return _dsState; }


private:

	D3D11_PRIMITIVE_TOPOLOGY _topology;
	ComPtr<ID3D11RasterizerState> _rsState;

	ComPtr<ID3D11BlendState> _blendState;
	//ComPtr<ID3D11SamplerState> _samplerState;
	static shared_ptr<SamplerState> _samplerState;
	ComPtr<ID3D11DepthStencilState> _dsState;

	float _blendFactor = 0.f;
	uint32 _sampleMask = 0xFFFFFFFF;
};

