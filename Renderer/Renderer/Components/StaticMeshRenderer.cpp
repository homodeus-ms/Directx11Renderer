#include "pch.h"
#include "StaticMeshRenderer.h"
#include "Actor/Actor.h"
#include "Structs/ShaderParameterTypes.h"
#include "Components/Transform.h"
#include "Components/CameraComponent.h"
#include "Resource/BasicMesh.h"
#include "Resource/Texture.h"
#include "Resource/Material.h"
#include "Resource/StaticMesh.h"
#include "Graphics/Buffer/InputLayout.h"
#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Buffer/IndexBuffer.h"
#include "Graphics/Shader/VertexShader.h"
#include "Graphics/Shader/PixelShader.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "Graphics/PipelineState/PipelineState.h"
#include "Managers/ShaderParameterManager.h"
#include "Graphics/PipelineState/SamplerState.h"

StaticMeshRenderer::StaticMeshRenderer()
	: Super(EComponentType::StaticMeshRenderer)
{
}

StaticMeshRenderer::~StaticMeshRenderer()
{
}

void StaticMeshRenderer::SetInputLayout()
{
	const vector<D3D11_INPUT_ELEMENT_DESC>& desc = StaticMeshVertexType::descs;
	_inputLayout = make_shared<InputLayout>();
	_inputLayout->Create(desc, _vertexShader->GetBlob());
}

void StaticMeshRenderer::SetShaderInfo(shared_ptr<ShaderInfo> shaderInfo)
{
	_shaderInfo = shaderInfo;
	SetVertexShader(_shaderInfo);
	SetPixelShader(_shaderInfo);
}

void StaticMeshRenderer::Construct()
{
	_bRenderReady = _shaderInfo != nullptr && _staticMesh != nullptr;
}

void StaticMeshRenderer::BeginPlay()
{
	Super::BeginPlay();
}

void StaticMeshRenderer::Render()
{
	Super::Render();

	// 한 StaticMesh의 SubMesh들이 다음 내용들을 똑같이 사용한다고 가정
	if (0)
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
				desc.MinLOD = 0.0f;
				desc.MaxLOD = D3D11_FLOAT32_MAX;
				desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

				// 샘플러 객체 생성
				HRESULT hr = DEVICE->CreateSamplerState(&desc, _samplerState.GetAddressOf());
				check(hr);

				// 슬롯 0에 바인딩
				CONTEXT->PSSetSamplers(0, 1, _samplerState.GetAddressOf());
			}

			auto rsState = _pipelineState->GetRsState();
			CONTEXT->RSSetState(rsState.Get());

			auto blendState = _pipelineState->GetBlendState();
			CONTEXT->OMSetBlendState(blendState.Get(), _pipelineState->GetBlendFactor(), _pipelineState->GetSampleMask());
		}

		SHADER_PARAM_MANAGER->PushGlobalData(CameraComponent::S_MatView, CameraComponent::S_MatProjection);
	}

	// Bone Matrixes
	BoneBuffer boneBuffer;
	const uint32 boneCount = _staticMesh->GetBoneCount();
	for (uint32 i = 0; i < boneCount; i++)
	{
		shared_ptr<ImportedStaticBone> bone = _staticMesh->GetBoneByIndex (i);
		boneBuffer.transforms[i] = bone->transform;
	}
	SHADER_PARAM_MANAGER->PushBoneBuffer(boneBuffer);

	// Transform
	auto world = GetOwnerTransform()->GetWorldMatrix();
	SHADER_PARAM_MANAGER->PushTransformData(TransformDesc{ world });


	// Mesh, Materials
	const auto& meshes = _staticMesh->GetMeshes();
	for (auto& mesh : meshes)
	{
		if (mesh->material)
		{
			SHADER_PARAM_MANAGER->PushMaterial(mesh->material);
		}

		// BoneIndex
		BoneIndex boneIndex;
		boneIndex.boneIndex = mesh->boneIndex;
		SHADER_PARAM_MANAGER->PushBoneIndex(boneIndex);

		SHADER_PARAM_MANAGER->BindAllDirtyBuffers();

		uint32 stride = mesh->vertexBuffer->GetStride();
		uint32 offset = mesh->vertexBuffer->GetOffset();

		CONTEXT->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
		CONTEXT->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

		DrawIndexed(mesh->indexBuffer->GetCount());
	}
}
