#pragma once

#include "Structs/ShaderParameterTypes.h"
#include "Structs/EFilterType.h"

class VertexUVBasicMesh;
class RenderTexture;
class InputLayout;
class VertexShader;
class PixelShader;
struct ShaderInfo;

class Filter
{
public:
	Filter(EFilterType type);
	virtual ~Filter();

	virtual void Tick() {};
	void Render();

	void Initialize(shared_ptr<ShaderInfo> shaderInfo, uint32 w = VIEW_X, uint32 h = VIEW_Y, uint32 viewportStartX = 0);
	ComPtr<ID3D11ShaderResourceView> GetSRV();
	FilterData& GetFilterData() { return _filterData; }
	D3D11_VIEWPORT& GetViewport() { return _viewport; }
	EFilterType GetFilterType() const { return _filterType; }

	void UseSelfRenderTarget();
	void SetShaderResources(vector<ComPtr<ID3D11ShaderResourceView>> SRVs);
	void SetRenderTargets(vector<ComPtr<ID3D11RenderTargetView>> RTVs);
	

public:
	void CreateViewport(uint32 w, uint32 h, uint32 topLeftX = 0);
	EFilterType _filterType{};
	shared_ptr<VertexUVBasicMesh> _filterQuad{};
	shared_ptr<RenderTexture> _texture{};

	D3D11_VIEWPORT _viewport{};

	shared_ptr<InputLayout> _inputLayout{};
	shared_ptr<VertexShader> _vertexShader{};
	shared_ptr<PixelShader> _pixelShader{};
	FilterData _filterData{};

	vector<ID3D11ShaderResourceView*> _SRVs{};
	vector<shared_ptr<SRVBindingInfo>> _SRVBindingInfos{};
	vector<ComPtr<ID3D11RenderTargetView>> _RTVs{};

};

