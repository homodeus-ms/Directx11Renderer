#include "pch.h"
#include "RenderComponentBase.h"
#include "Actor/Actor.h"
#include "Actor/LightActor.h"
#include "Components/Transform.h"
#include "Components/CameraComponent.h"
#include "Resource/BasicMesh/BasicMesh.h"
#include "Resource/Texture/LoadedTexture.h"
#include "Graphics/Buffer/InputLayout.h"
#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Buffer/IndexBuffer.h"
#include "Graphics/Shader/VertexShader.h"
#include "Graphics/Shader/GeometryShader.h"
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
	CONTEXT->IASetInputLayout(_inputLayout->GetComPtr().Get());
	CONTEXT->IASetPrimitiveTopology(_defaultStates->_topology);

	if (_vertexShader)
		CONTEXT->VSSetShader(_vertexShader->GetComPtr().Get(), nullptr, 0);
	if (_pixelShader)
		CONTEXT->PSSetShader(_pixelShader->GetComPtr().Get(), nullptr, 0);

	CONTEXT->PSSetSamplers(0, 1, _defaultStates->_samplerState.GetAddressOf());
	CONTEXT->RSSetState(_defaultStates->_rsState.Get());
	CONTEXT->OMSetBlendState(_defaultStates->_blendState.Get(), &_defaultStates->_blendFactor, _defaultStates->_sampleMask);
}

void RenderComponentBase::RenderDepthOnly(bool bForPointLight, int32 instanceCount)
{
	CONTEXT->IASetInputLayout(_shadowMapResources.inputLayout->GetComPtr().Get());
	CONTEXT->IASetPrimitiveTopology(_shadowStates->_topology);

	if (bForPointLight)
	{
		CONTEXT->VSSetShader(_shadowMapResources.pointLightVertexShader->GetComPtr().Get(), nullptr, 0);
		CONTEXT->GSSetShader(_shadowMapResources.pointLightGeometryShader->GetComPtr().Get(), nullptr, 0);
		CONTEXT->PSSetShader(_shadowMapResources.pointLightPixelShader->GetComPtr().Get(), nullptr, 0);
	}
	else
	{
		CONTEXT->VSSetShader(_shadowMapResources.defaultVertexShader->GetComPtr().Get(), nullptr, 0);
	}
	
	CONTEXT->RSSetState(_shadowStates->_rsState.Get());
	CONTEXT->OMSetDepthStencilState(_shadowStates->_dsState.Get(), 0);
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
	_defaultStates = PipelineState::GetDefaultStates();
	_shadowStates = PipelineState::GetShadowStates();
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
	CONTEXT->DrawIndexedInstanced(indexCountPerInstance, indexCountPerInstance, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

void RenderComponentBase::ClearGeometryShader()
{
	CONTEXT->GSSetShader(nullptr, 0, 0);
}

