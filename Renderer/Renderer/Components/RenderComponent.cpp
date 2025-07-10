#include "pch.h"
#include "RenderComponent.h"
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


RenderComponent::RenderComponent() : Super(ComponentType::MeshRenderer)
{
}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::SetMesh(const shared_ptr<Mesh>& mesh)
{
	_mesh = mesh;
	_bHasMesh = true;
}

void RenderComponent::SetMaterial(const shared_ptr<Material>& material)
{
	_material = material;
	SetVertexShader();
	SetPixelShader();
	_bHasMaterial = true;
}


void RenderComponent::BeginPlay()
{
	Super::BeginPlay();

	if (_bHasMesh && _bHasMaterial)
	{
		SetInputLayout();
		GetDefaultStates();

		TransformDesc desc;
		desc.W = GetOwnerTransform()->GetWorldMatrix();
		SHADER_PARAM_MANAGER->PushTransformData(desc);
	}
}


void RenderComponent::Render()
{
	if (!_bHasMesh || !_bHasMaterial)
		return;

	//if (GetOwner()->IsTransformChanged())
	{
		TransformDesc desc;
		desc.W = GetOwnerTransform()->GetWorldMatrix();
		SHADER_PARAM_MANAGER->PushTransformData(desc);
		SHADER_PARAM_MANAGER->Update();

		/*{
			LightDesc lightDesc;
			lightDesc.ambient = Vec4(0.5);
			lightDesc.diffuse = Vec4(1.f);
			lightDesc.specular = Vec4(1.f);
			lightDesc.direction = Vec3(0.f, 0.f, -1.f);
			SHADER_PARAM_MANAGER->PushLightData(lightDesc);
		}*/

		UpdatePipeline();
	}
}

void RenderComponent::UpdatePipeline()
{
	// IA
	CONTEXT->IASetInputLayout(_inputLayout->GetComPtr().Get());
	CONTEXT->IASetPrimitiveTopology(_pipelineState->GetTopology());
	SetVertexBuffer();
	SetIndexBuffer();
	
	// VS
	if (_vertexShader)
		CONTEXT->VSSetShader(_vertexShader->GetComPtr().Get(), nullptr, 0);

	// SRV, Constant Buffers
	if (_material != nullptr)
	{
		SHADER_PARAM_MANAGER->PushMaterial(_material);
	}
	SHADER_PARAM_MANAGER->BindAll();

	// RS
	CONTEXT->RSSetState(_pipelineState->GetRsState().Get());

	// PS
	if (_pixelShader)
		CONTEXT->PSSetShader(_pixelShader->GetComPtr().Get(), nullptr, 0);

	// OM
	CONTEXT->OMSetBlendState(_pipelineState->GetBlendState().Get(), _pipelineState->GetBlendFactor(), _pipelineState->GetSampleMask());
	 
	DrawIndexed(_mesh->GetIndexBuffer()->GetCount());
}

void RenderComponent::SetInputLayout()
{
	const vector<D3D11_INPUT_ELEMENT_DESC>& desc = _mesh->GetInputLayoutDesc();
	_inputLayout = make_shared<InputLayout>();
	_inputLayout->Create(desc, _vertexShader->GetBlob());
}

void RenderComponent::SetVertexShader()
{
	const shared_ptr<ShaderInfo>& info = _material->_shaderInfo;
	_vertexShader = make_shared<VertexShader>();
	_vertexShader->Create(info->_shaderPath, info->_vsEntryName, info->_vsVersion);
}

void RenderComponent::SetPixelShader()
{
	const shared_ptr<ShaderInfo>& info = _material->_shaderInfo;
	_pixelShader = make_shared<PixelShader>();
	_pixelShader->Create(info->_shaderPath, info->_psEntryName, info->_psVersion);
}

void RenderComponent::GetDefaultStates()
{
	_pipelineState = PipelineState::GetDefaultState();
}

void RenderComponent::SetVertexBuffer()
{
	const shared_ptr<VertexBuffer>& buffer = _mesh->GetVertexBuffer();
	uint32 stride = buffer->GetStride();
	uint32 offset = buffer->GetOffset();
	CONTEXT->IASetVertexBuffers(0, 1, buffer->GetComPtr().GetAddressOf(), &stride, &offset);
}

void RenderComponent::SetIndexBuffer()
{
	const shared_ptr<IndexBuffer>& buffer = _mesh->GetIndexBuffer();
	CONTEXT->IASetIndexBuffer(buffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
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

