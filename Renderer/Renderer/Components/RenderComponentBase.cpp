#include "pch.h"
#include "RenderComponentBase.h"
#include "Actor/Actor.h"
#include "Actor/LightActor.h"
#include "Components/Transform.h"
#include "Components/CameraComponent.h"
#include "Resource/BasicMesh/BasicMesh.h"
#include "Resource/Texture.h"
#include "Graphics/Buffer/InputLayout.h"
#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Buffer/IndexBuffer.h"
#include "Graphics/Shader/VertexShader.h"
#include "Graphics/Shader/PixelShader.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "Graphics/PipelineState/PipelineState.h"
#include "Managers/ShaderParameterManager.h"
#include "Resource/Material.h"
#include "Graphics/PipelineState/SamplerState.h"
#include "Graphics/RenderPass/ShadowMap.h"


RenderComponentBase::RenderComponentBase(EComponentType componentType) 
	: Super(componentType)
{
}

RenderComponentBase::~RenderComponentBase()
{

}

void RenderComponentBase::BeginPlay()
{
	Super::BeginPlay();

	if (_bRenderReady)
	{
		SetInputLayout();
		GetDefaultStates();
	}

	_shadowMapResources = GRAPHICS->GetShadowMap()->GetShadowMapResources();
}


void RenderComponentBase::Render()
{
	{
		CONTEXT->IASetInputLayout(_inputLayout->GetComPtr().Get());
		CONTEXT->IASetPrimitiveTopology(_pipelineState->GetTopology());

		if (_vertexShader)
			CONTEXT->VSSetShader(_vertexShader->GetComPtr().Get(), nullptr, 0);
		if (_pixelShader)
			CONTEXT->PSSetShader(_pixelShader->GetComPtr().Get(), nullptr, 0);

		// Test
		{
			D3D11_SAMPLER_DESC desc = {};
			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.BorderColor[0] = 100.f;
			desc.MinLOD = 0.0f;
			desc.MaxLOD = D3D11_FLOAT32_MAX;
			desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

			// »ùÇÃ·¯ °´Ã¼ »ý¼º
			HRESULT hr = DEVICE->CreateSamplerState(&desc, _samplerState.GetAddressOf());
			check(hr);

			// ½½·Ô 0¿¡ ¹ÙÀÎµù
			CONTEXT->PSSetSamplers(0, 1, _samplerState.GetAddressOf());
		}

		auto rsState = _pipelineState->GetRsState();
		CONTEXT->RSSetState(rsState.Get());

		auto blendState = _pipelineState->GetBlendState();
		CONTEXT->OMSetBlendState(blendState.Get(), _pipelineState->GetBlendFactor(), _pipelineState->GetSampleMask());
	}
}

void RenderComponentBase::RenderDepthOnly(bool bForPointLight)
{
	CONTEXT->IASetInputLayout(_shadowMapResources.inputLayout->GetComPtr().Get());
	CONTEXT->IASetPrimitiveTopology(_pipelineState->GetTopology());

	if (bForPointLight)
	{
		CONTEXT->VSSetShader(_shadowMapResources.pointLightVertexShader->GetComPtr().Get(), nullptr, 0);
		CONTEXT->PSSetShader(_shadowMapResources.pointLightPixelShader->GetComPtr().Get(), nullptr, 0);
	}
	else
	{
		CONTEXT->VSSetShader(_shadowMapResources.defaultVertexShader->GetComPtr().Get(), nullptr, 0);
	}


	CONTEXT->RSSetState(_shadowMapResources.rasterizerState.Get());
	CONTEXT->OMSetDepthStencilState(_shadowMapResources.depthStencilState.Get(), 0);

	//{
	//	D3D11_RASTERIZER_DESC desc = {};
	//	desc.FillMode = D3D11_FILL_SOLID;
	//	desc.CullMode = D3D11_CULL_NONE;  // ¶Ç´Â D3D11_CULL_FRONT
	//	desc.DepthClipEnable = TRUE;
	//
	//	DEVICE->CreateRasterizerState(&desc, _tempRS.GetAddressOf());
	//	CONTEXT->RSSetState(_tempRS.Get());
	//}
	//
	//{
	//	D3D11_SAMPLER_DESC desc = {};
	//	desc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	//	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	//	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	//	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	//	desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	//
	//	DEVICE->CreateSamplerState(&desc, _tempSS.GetAddressOf());
	//	CONTEXT->PSSetSamplers(0, 1, _tempSS.GetAddressOf());
	//}
}

void RenderComponentBase::SetVertexShader(shared_ptr<ShaderInfo> shaderInfo)
{
	_vertexShader = make_shared<VertexShader>();
	_vertexShader->Create(shaderInfo->_shaderPath, shaderInfo->_vsEntryName, shaderInfo->_vsVersion);
}

void RenderComponentBase::SetPixelShader(shared_ptr<ShaderInfo> shaderInfo)
{
	_pixelShader = make_shared<PixelShader>();
	_pixelShader->Create(shaderInfo->_shaderPath, shaderInfo->_psEntryName, shaderInfo->_psVersion);
}

void RenderComponentBase::GetDefaultStates()
{
	_pipelineState = PipelineState::GetDefaultState();
}

void RenderComponentBase::Draw(UINT vertexCount, UINT startVertexLocation)
{
	CONTEXT->Draw(vertexCount, startVertexLocation);
}

void RenderComponentBase::DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
{
	CONTEXT->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

void RenderComponentBase::DrawInstanced(UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation, UINT startInstanceLocation)
{
}

void RenderComponentBase::DrawIndexedInstanced(UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation, INT baseVertexLocation, UINT startInstanceLocation)
{
}

