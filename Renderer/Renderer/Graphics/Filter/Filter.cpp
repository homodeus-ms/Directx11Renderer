#include "pch.h"
#include "Filter.h"
#include "Resource/BasicMesh/VertexUVBasicMesh.h"
#include "Graphics/Shader/Shader.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "Graphics/Buffer/InputLayout.h"
#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Buffer/IndexBuffer.h"
#include "Graphics/Shader/VertexShader.h"
#include "Graphics/Shader/PixelShader.h"
#include "Resource/Texture/FilterTexture.h"
#include "Managers/ShaderParameterManager.h"


Filter::Filter(EFilterType type)
	: _filterType(type)
{

}

Filter::~Filter()
{
}

void Filter::Initialize(shared_ptr<ShaderInfo> shaderInfo, uint32 w, uint32 h)
{
	_filterQuad = make_shared<VertexUVBasicMesh>();
	_filterQuad->CreateQuad();

	_vertexShader = make_shared<VertexShader>();
	_vertexShader->Create(shaderInfo->_vsShaderPath, shaderInfo->_vsEntryName, shaderInfo->_vsVersion);
	_pixelShader = make_shared<PixelShader>();
	_pixelShader->Create(shaderInfo->_psShaderPath, shaderInfo->_psEntryName, shaderInfo->_psVersion);

	const vector<D3D11_INPUT_ELEMENT_DESC>& desc = _filterQuad->GetInputLayoutDesc();
	_inputLayout = make_shared<InputLayout>();
	_inputLayout->Create(desc, _vertexShader->GetBlob());
	
	CreateViewport(w, h);

	_texture = make_shared<FilterTexture>();
	_texture->CreateTexture(w, h);

	_filterData.dx = 1.f / w;
	_filterData.dy = 1.f / h;

	ComPtr<ID3D11RenderTargetView> myRTV = _texture->GetRTV();
	_RTVs.push_back(myRTV);
}

ComPtr<ID3D11ShaderResourceView> Filter::GetSRV()
{
	return _texture->GetSRV();
}

void Filter::UseSelfRenderTarget()
{
	SetRenderTargets({ _texture->GetRTV() });
}

void Filter::SetShaderResources(vector<ComPtr<ID3D11ShaderResourceView>> SRVs)
{
	_SRVs.clear();
	for (auto& srv : SRVs)
		_SRVs.push_back(srv.Get());
}

void Filter::SetRenderTargets(vector<ComPtr<ID3D11RenderTargetView>> RTVs)
{
	_RTVs.clear();
	for (auto& rtv : RTVs)
		_RTVs.push_back(rtv);
}

void Filter::Render()
{
	Tick();

	assert(_SRVs.size() > 0);
	assert(_RTVs.size() > 0);
	
	CONTEXT->OMSetRenderTargets(UINT(_RTVs.size()), _RTVs.data()->GetAddressOf(), nullptr);
	//float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//CONTEXT->ClearRenderTargetView(_RTVs.data()->Get(), clearColor);
	CONTEXT->RSSetViewports(1, &_viewport);

	CONTEXT->IASetInputLayout(_inputLayout->GetComPtr().Get());
	if (_vertexShader)
		CONTEXT->VSSetShader(_vertexShader->GetComPtr().Get(), nullptr, 0);
	if (_pixelShader)
		CONTEXT->PSSetShader(_pixelShader->GetComPtr().Get(), nullptr, 0);

	// TEMP : ShaderParameterManager 를 사용하게 하는 게 좋을 것 같은데?
	uint8 slot = static_cast<uint8>(EFilterTextureType::Filtered);
	CONTEXT->PSSetShaderResources(slot, UINT(_SRVs.size()), _SRVs.data());
	SHADER_PARAM_MANAGER->PushFilterData(_filterData);

	uint32 stride = _filterQuad->GetVertexBuffer()->GetStride();
	uint32 offset = _filterQuad->GetVertexBuffer()->GetOffset();

	CONTEXT->IASetVertexBuffers(0, 1, _filterQuad->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
	CONTEXT->IASetIndexBuffer(_filterQuad->GetIndexBuffer()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	CONTEXT->DrawIndexed(_filterQuad->GetIndexBuffer()->GetCount(), 0, 0);
	
}

void Filter::CreateViewport(uint32 w, uint32 h)
{
	ZeroMemory(&_viewport, sizeof(D3D11_VIEWPORT));
	
	_viewport.TopLeftX = 0;
	_viewport.TopLeftY = 0;
	_viewport.Width = static_cast<float>(w);
	_viewport.Height = static_cast<float>(h);
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
}
