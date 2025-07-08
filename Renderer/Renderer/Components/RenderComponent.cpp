#include "pch.h"
#include "RenderComponent.h"
#include "Actor/Actor.h"
#include "Components/Transform.h"
#include "Components/CameraComponent.h"
#include "Engine/Resource/Mesh.h"
#include "Engine/Resource/Texture.h"
#include "Graphics/Buffer/InputLayout.h"
#include "Graphics/Shader/VertexShader.h"
#include "Graphics/Shader/PixelShader.h"
#include "Graphics/Shader/ShaderInfos.h"
#include "Graphics/PipelineState/PipelineState.h"
#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Buffer/IndexBuffer.h"



RenderComponent::RenderComponent() : Super(ComponentType::MeshRenderer)
{
}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::SetStaticMeshInfo(const StaticMeshInfo& info)
{
	_staticMeshInfo = info;
	SetVertexShader();
	SetPixelShader();
	SetInputLayout();
	GetDefaultStates();
	bHasInfo = true;
}


void RenderComponent::Construct()
{
	Super::Construct();

	_transformBuffer = make_shared<ConstantBuffer<TransformData>>();
	_transformBuffer->Create();
}

void RenderComponent::BeginPlay()
{
	Super::BeginPlay();
}

void RenderComponent::Tick()
{
	Super::Tick();

	if (GetOwner()->IsTransformChanged())
	{
		_transformData.matWorld = Matrix::Identity; // GetOwnerTransform()->GetWorldMatrix();
		_transformData.matView = CameraComponent::S_MatView;
		_transformData.matProjection = CameraComponent::S_MatProjection;
		_transformBuffer->CopyData(_transformData);
		_transformBufferDirty = true;
	}
}

void RenderComponent::Render()
{
	if (!bHasInfo)
		return;

	UpdatePipeline();
}

void RenderComponent::UpdatePipeline()
{
	// IA
	CONTEXT->IASetInputLayout(_inputLayout->GetComPtr().Get());
	CONTEXT->IASetPrimitiveTopology(_pipelineState->GetTopology());

	// VS
	SetVertexBuffer();
	SetIndexBuffer();
	
	if (_staticMeshInfo.texture)
		SetTexture(0, EShaderStage::PsStage, _staticMeshInfo.texture);

	if (_vertexShader)
		CONTEXT->VSSetShader(_vertexShader->GetComPtr().Get(), nullptr, 0);

	// TEMP
	if (_transformBufferDirty)
	{
		CONTEXT->VSSetConstantBuffers(0, 1, _transformBuffer->GetComPtr().GetAddressOf());
		_transformBufferDirty = false;
	}

	// RS
	CONTEXT->RSSetState(_pipelineState->GetRsState().Get());

	// PS
	if (_pixelShader)
		CONTEXT->PSSetShader(_pixelShader->GetComPtr().Get(), nullptr, 0);

	// OM
	CONTEXT->OMSetBlendState(_pipelineState->GetBlendState().Get(), _pipelineState->GetBlendFactor(), _pipelineState->GetSampleMask());
	 
	DrawIndexed(_staticMeshInfo.mesh->GetIndexBuffer()->GetCount());
}

void RenderComponent::SetInputLayout()
{
	const vector<D3D11_INPUT_ELEMENT_DESC>& desc = _staticMeshInfo.mesh->GetInputLayoutDesc();
	_inputLayout = make_shared<InputLayout>();
	_inputLayout->Create(desc, _vertexShader->GetBlob());
}

void RenderComponent::SetVertexShader()
{
	const shared_ptr<ShaderInfos>& info = _staticMeshInfo.shaderInfos;
	_vertexShader = make_shared<VertexShader>();
	_vertexShader->Create(info->_shaderPath, info->_vsEntryName, info->_vsVersion);
}

void RenderComponent::SetPixelShader()
{
	const shared_ptr<ShaderInfos>& info = _staticMeshInfo.shaderInfos;
	_pixelShader = make_shared<PixelShader>();
	_pixelShader->Create(info->_shaderPath, info->_psEntryName, info->_psVersion);
}

void RenderComponent::GetDefaultStates()
{
	_pipelineState = PipelineState::GetDefaultState();
}

void RenderComponent::SetVertexBuffer()
{
	const shared_ptr<VertexBuffer>& buffer = _staticMeshInfo.mesh->GetVertexBuffer();
	uint32 stride = buffer->GetStride();
	uint32 offset = buffer->GetOffset();
	CONTEXT->IASetVertexBuffers(0, 1, buffer->GetComPtr().GetAddressOf(), &stride, &offset);
}

void RenderComponent::SetIndexBuffer()
{
	const shared_ptr<IndexBuffer>& buffer = _staticMeshInfo.mesh->GetIndexBuffer();
	CONTEXT->IASetIndexBuffer(buffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
}

void RenderComponent::SetTexture(uint32 slot, EShaderStage stage, shared_ptr<Texture> texture)
{
	if (IsStageVS(stage))
		CONTEXT->VSSetShaderResources(slot, 1, texture->GetComPtr().GetAddressOf());

	if (IsStagePS(stage))
		CONTEXT->PSSetShaderResources(slot, 1, texture->GetComPtr().GetAddressOf());

}

void RenderComponent::Draw(UINT vertexCount, UINT startVertexLocation)
{
	CONTEXT->Draw(vertexCount, startVertexLocation);
}

void RenderComponent::DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
{
	CONTEXT->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

void RenderComponent::DrawInstanced(UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation, UINT startInstanceLocation)
{
}

void RenderComponent::DrawIndexedInstanced(UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation, INT baseVertexLocation, UINT startInstanceLocation)
{
}

