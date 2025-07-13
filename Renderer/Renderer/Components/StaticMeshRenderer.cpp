#include "pch.h"
#include "StaticMeshRenderer.h"
#include "Actor/Actor.h"
#include "Structs/ShaderParameterTypes.h"
#include "Components/Transform.h"
#include "Components/CameraComponent.h"
#include "Resource/Mesh.h"
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

StaticMeshRenderer::StaticMeshRenderer()
	: Super(EComponentType::StaticMeshRenderer)
{
}

StaticMeshRenderer::~StaticMeshRenderer()
{
}

void StaticMeshRenderer::SetShaderInfo(shared_ptr<ShaderInfo> shaderInfo)
{
	_shaderInfo = shaderInfo;
	SetVertexShader(_shaderInfo);
	SetPixelShader(_shaderInfo);
}

void StaticMeshRenderer::Render()
{
	if (_staticMesh == nullptr || _shaderInfo == nullptr)
		return;

	// 한 StaticMesh의 SubMesh들이 다음 내용들을 똑같이 사용한다고 가정
	{
		CONTEXT->IASetInputLayout(_inputLayout->GetComPtr().Get());
		CONTEXT->IASetPrimitiveTopology(_pipelineState->GetTopology());

		if (_vertexShader)
			CONTEXT->VSSetShader(_vertexShader->GetComPtr().Get(), nullptr, 0);
		if (_pixelShader)
			CONTEXT->PSSetShader(_pixelShader->GetComPtr().Get(), nullptr, 0);

		CONTEXT->RSSetState(_pipelineState->GetRsState().Get());
		CONTEXT->OMSetBlendState(_pipelineState->GetBlendState().Get(), _pipelineState->GetBlendFactor(), _pipelineState->GetSampleMask());
	}

	// Temp
	{
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(1.f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(1.f, 1.f, 1.f, 1.f);
		lightDesc.direction = Vec3(0.f, -1.f, 0.f);
		SHADER_PARAM_MANAGER->PushLightData(lightDesc);
	}
	
	SHADER_PARAM_MANAGER->PushGlobalData(CameraComponent::S_MatView, CameraComponent::S_MatProjection);

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
			mesh->material->GetMaterialDesc().ambient = Vec4(0.6f);
			mesh->material->GetMaterialDesc().ambient.z = 1.f;

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
