#include "pch.h"
#include "StaticMeshRenderer.h"
#include "Actor/Actor.h"
#include "Structs/ShaderParameterTypes.h"
#include "Components/Transform.h"
#include "Components/CameraComponent.h"
#include "Resource/BasicMesh/BasicMesh.h"
#include "Resource/Texture/LoadedTexture.h"
#include "Resource/Material/MaterialBase.h"
#include "Resource/Material/Material.h"

#include "Resource/StaticMesh.h"
#include "Graphics/Buffer/InputLayout.h"
#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Buffer/IndexBuffer.h"
#include "Graphics/Shader/VertexShader.h"
#include "Graphics/Shader/PixelShader.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "Graphics/PipelineState/PipelineState.h"
#include "Managers/ShaderParameterManager.h"
#include "Managers/SceneManager.h"
#include "Graphics/PipelineState/SamplerState.h"

StaticMeshRenderer::StaticMeshRenderer()
	: Super(EComponentType::StaticMeshRenderer)
{
}

StaticMeshRenderer::~StaticMeshRenderer()
{
}

vector<shared_ptr<MaterialBase>> StaticMeshRenderer::GetMaterials()
{
	return _staticMesh->GetMaterials();
}

void StaticMeshRenderer::ChangeMaterialType(EMaterialType type)
{
	_staticMesh->ChangeMaterialType(type);
}

void StaticMeshRenderer::Construct()
{
	_bRenderReady = _staticMesh != nullptr;
}

void StaticMeshRenderer::BeginPlay()
{
	Super::BeginPlay();
}

void StaticMeshRenderer::Render()
{
	Super::Render();

	// Bone Matrixes
	//BoneBuffer boneBuffer;
	//const uint32 boneCount = _staticMesh->GetBoneCount();
	//for (uint32 i = 0; i < boneCount; i++)
	//{
	//	shared_ptr<ImportedBone> bone = _staticMesh->GetBoneByIndex (i);
	//	boneBuffer.transforms[i] = bone->transform;
	//}
	//SHADER_PARAM_MANAGER->PushBoneBuffer(boneBuffer);

	// Transform
	auto world = GetOwnerTransform()->GetWorldMatrix();
	auto invWorld = GetOwnerTransform()->GetInvWorldMatrix();
	SHADER_PARAM_MANAGER->PushTransformData(TransformDesc{ world, invWorld });

	// Mesh, Materials
	const auto& meshes = _staticMesh->GetMeshes();
	for (int32 i = 0; i < meshes.size(); ++i)
	{
		auto mesh = meshes[i];

		if (mesh->material)
		{
			SHADER_PARAM_MANAGER->PushMaterial(mesh->material);
		}

		// BoneIndex
		//BoneIndex boneIndex;
		//boneIndex.boneIndex = mesh->boneIndex;
		//SHADER_PARAM_MANAGER->PushBoneIndex(boneIndex);

		SHADER_PARAM_MANAGER->BindAllDirtyBuffers();

		uint32 stride = mesh->vertexBuffer->GetStride();
		uint32 offset = mesh->vertexBuffer->GetOffset();

		CONTEXT->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
		CONTEXT->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

		DrawIndexed(mesh->indexBuffer->GetCount());
	}
}

void StaticMeshRenderer::RenderDepthOnly(bool bForPointLight, int32 instanceCount)
{
	Super::RenderDepthOnly(bForPointLight, instanceCount);

	// Bone Matrixes
	//BoneBuffer boneBuffer;
	//const uint32 boneCount = _staticMesh->GetBoneCount();
	//for (uint32 i = 0; i < boneCount; i++)
	//{
	//	shared_ptr<ImportedBone> bone = _staticMesh->GetBoneByIndex(i);
	//	boneBuffer.transforms[i] = bone->transform;
	//}
	//SHADER_PARAM_MANAGER->PushBoneBuffer(boneBuffer);

	// Transform
	auto world = GetOwnerTransform()->GetWorldMatrix();
	auto invWorld = GetOwnerTransform()->GetInvWorldMatrix();
	SHADER_PARAM_MANAGER->PushTransformData(TransformDesc{ world, invWorld });
	SHADER_PARAM_MANAGER->BindAllDirtyBuffers();

	// Mesh, Materials
	const auto& meshes = _staticMesh->GetMeshes();
	uint32 meshsize = static_cast<uint32>(meshes.size());
	for (uint32 i = 0; i < meshsize; ++i)
	{
		auto mesh = meshes[i];
		// BoneIndex
		//BoneIndex boneIndex;
		//boneIndex.boneIndex = mesh->boneIndex;
		//SHADER_PARAM_MANAGER->PushBoneIndex(boneIndex);

		uint32 stride = sizeof(VertexData);
		uint32 offset = 0;

		// Position만 가진 VectexBuffer를 Set
		CONTEXT->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetPosOnlyBuffer().GetAddressOf(), &stride, &offset);
		CONTEXT->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
		
		DrawIndexed(mesh->indexBuffer->GetCount());
	}
}

void StaticMeshRenderer::RenderDrawNormal()
{
	auto world = GetOwnerTransform()->GetWorldMatrix();
	auto invWorld = GetOwnerTransform()->GetInvWorldMatrix();
	SHADER_PARAM_MANAGER->PushTransformData(TransformDesc{ world, invWorld });

	// Mesh, Materials
	const auto& meshes = _staticMesh->GetMeshes();
	for (int32 i = 0; i < meshes.size(); ++i)
	{
		auto mesh = meshes[i];
		SHADER_PARAM_MANAGER->BindAllDirtyBuffers();

		uint32 stride = mesh->vertexBuffer->GetStride();
		uint32 offset = mesh->vertexBuffer->GetOffset();
		CONTEXT->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
		
		Draw(mesh->vertexBuffer->GetCount(), 0);
	}
}


