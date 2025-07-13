#include "pch.h"
#include "RenderComponentBase.h"
#include "Actor/Actor.h"
#include "Components/Transform.h"
#include "Components/CameraComponent.h"
#include "Resource/Mesh.h"
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


RenderComponentBase::RenderComponentBase(EComponentType componentType) 
	: Super(componentType)
{
}

RenderComponentBase::~RenderComponentBase()
{

}

void RenderComponentBase::SetMesh(const shared_ptr<Mesh>& mesh)
{
	_mesh = mesh;
	_bHasMesh = true;
}

void RenderComponentBase::SetMaterial(const shared_ptr<Material>& material)
{
	_material = material;
	SetVertexShader(_material->_shaderInfo);
	SetPixelShader(_material->_shaderInfo);
	_bHasMaterial = true;
}


void RenderComponentBase::BeginPlay()
{
	Super::BeginPlay();

	//if (_bHasMesh && _bHasMaterial)
	{
		SetInputLayout();
		GetDefaultStates();

		TransformDesc desc;
		desc.W = GetOwnerTransform()->GetWorldMatrix();
		SHADER_PARAM_MANAGER->PushTransformData(desc);
	}
}


void RenderComponentBase::Render()
{
	if (!_bHasMesh || !_bHasMaterial)
		return;

	// IA
	CONTEXT->IASetInputLayout(_inputLayout->GetComPtr().Get());
	CONTEXT->IASetPrimitiveTopology(_pipelineState->GetTopology());

	// VS
	if (_vertexShader)
		CONTEXT->VSSetShader(_vertexShader->GetComPtr().Get(), nullptr, 0);

	// SRV, Constant Buffers

	{
		TransformDesc desc;
		desc.W = GetOwnerTransform()->GetWorldMatrix();
		SHADER_PARAM_MANAGER->PushTransformData(desc);
	}

	if (_material != nullptr)
	{
		SHADER_PARAM_MANAGER->PushMaterial(_material);
	}
	// SHADER_PARAM_MANAGER->BindAll();

	// RS
	CONTEXT->RSSetState(_pipelineState->GetRsState().Get());

	// PS
	if (_pixelShader)
		CONTEXT->PSSetShader(_pixelShader->GetComPtr().Get(), nullptr, 0);

	// OM
	CONTEXT->OMSetBlendState(_pipelineState->GetBlendState().Get(), _pipelineState->GetBlendFactor(), _pipelineState->GetSampleMask());

	SetVertexBuffer();
	SetIndexBuffer();

	DrawIndexed(_mesh->GetIndexBuffer()->GetCount());
	
}


void RenderComponentBase::SetInputLayout()
{
	const vector<D3D11_INPUT_ELEMENT_DESC>& desc = _mesh->GetInputLayoutDesc();
	_inputLayout = make_shared<InputLayout>();
	_inputLayout->Create(desc, _vertexShader->GetBlob());
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

void RenderComponentBase::SetVertexBuffer()
{
	const shared_ptr<VertexBuffer>& buffer = _mesh->GetVertexBuffer();
	uint32 stride = buffer->GetStride();
	uint32 offset = buffer->GetOffset();
	CONTEXT->IASetVertexBuffers(0, 1, buffer->GetComPtr().GetAddressOf(), &stride, &offset);
}

void RenderComponentBase::SetIndexBuffer()
{
	const shared_ptr<IndexBuffer>& buffer = _mesh->GetIndexBuffer();
	CONTEXT->IASetIndexBuffer(buffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
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

