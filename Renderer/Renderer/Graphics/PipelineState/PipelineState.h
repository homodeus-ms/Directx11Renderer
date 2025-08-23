#pragma once

struct PipelineState
{
public:
	PipelineState();
	~PipelineState();

	static PipelineState* GetDefaultStates();
	static PipelineState* GetShadowStates();
	static PipelineState* GetFilterStates();
	static PipelineState* GetDrawStencilStates();
	static PipelineState* GetUsingStencilStates();

	D3D11_PRIMITIVE_TOPOLOGY _topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	ComPtr<ID3D11RasterizerState> _rsState;
	ComPtr<ID3D11RasterizerState> _wireframeState;

	ComPtr<ID3D11BlendState> _blendState;
	ComPtr<ID3D11SamplerState> _samplerState;
	ComPtr<ID3D11SamplerState> _clampSamplerState;
	ComPtr<ID3D11DepthStencilState> _dsState;

	float _blendFactor = 0.f;
	uint32 _sampleMask = 0xFFFFFFFF;

	

public:
	
	static void CreateDescs();
	static bool s_bDescsSet;

	static PipelineState* s_defaultStates;
	static PipelineState* s_shadowStates;
	static PipelineState* s_filterStates;
	static PipelineState* s_drawStencilStates;
	static PipelineState* s_usingStencilStates;

	// Sampler
	static D3D11_SAMPLER_DESC DEFAULT_SAMPLER_DESC;
	static D3D11_SAMPLER_DESC CLAMP_SAMPLER_DESC;
	static D3D11_SAMPLER_DESC FILTER_SAMPLER_DESC;
	static D3D11_SAMPLER_DESC COMPARISION_SAMPLER_DESC;

	// RS
	static D3D11_RASTERIZER_DESC DEFAULT_RASTERIZER_DESC;
	static D3D11_RASTERIZER_DESC CCW_RASTERIZER_DESC;
	static D3D11_RASTERIZER_DESC SHADOW_RASTERIZER_DESC;
	static D3D11_RASTERIZER_DESC FILTER_RASTERIZER_DESC;
	
	// Depth Stencil
	static D3D11_DEPTH_STENCIL_DESC DEFAULT_DEPTH_STENCIL_DESC;
	static D3D11_DEPTH_STENCIL_DESC DRAW_STENCIL_DESC;
	static D3D11_DEPTH_STENCIL_DESC USING_STENCIL_DESC;
	static D3D11_DEPTH_STENCIL_DESC SHADOW_DEPTH_STENCIL_DESC;

	// Blend
	static D3D11_RENDER_TARGET_BLEND_DESC DEFAULT_RENDER_TARGET_BLEND_DESC;
	static D3D11_BLEND_DESC DEFAULT_BLEND_DESC;
	static D3D11_BLEND_DESC MIRROR_BLEND_DESC;

	
};

