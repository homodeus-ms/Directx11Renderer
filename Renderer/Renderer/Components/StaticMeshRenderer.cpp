#include "pch.h"
#include "StaticMeshRenderer.h"
#include "Actor/Actor.h"
#include "Structs/ShaderParameterTypes.h"
#include "Components/Transform.h"
#include "Components/CameraComponent.h"
#include "Resource/BasicMesh/BasicMesh.h"
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
#include "Managers/SceneManager.h"
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

vector<shared_ptr<Material>> StaticMeshRenderer::GetMaterials()
{
	return _staticMesh->GetMaterials();
}

void StaticMeshRenderer::SetShaderInfo(shared_ptr<ShaderInfo> shaderInfo)
{
	_shaderInfo = shaderInfo;
	SetVertexShader(_shaderInfo);
	SetPixelShader(_shaderInfo);
}

void StaticMeshRenderer::ChangeMaterialType(EMaterialType type)
{
	_staticMesh->ChangeMaterialType(type);
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

void StaticMeshRenderer::RenderDepthOnly(bool bForPointLight)
{
	Super::RenderDepthOnly(bForPointLight);

	// Bone Matrixes
	BoneBuffer boneBuffer;
	const uint32 boneCount = _staticMesh->GetBoneCount();
	for (uint32 i = 0; i < boneCount; i++)
	{
		shared_ptr<ImportedStaticBone> bone = _staticMesh->GetBoneByIndex(i);
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
		// BoneIndex
		BoneIndex boneIndex;
		boneIndex.boneIndex = mesh->boneIndex;
		SHADER_PARAM_MANAGER->PushBoneIndex(boneIndex);
		SHADER_PARAM_MANAGER->BindAllDirtyBuffers();

		uint32 stride = sizeof(VertexData);
		uint32 offset = 0;

		// Position만 가진 VectexBuffer를 Set
		CONTEXT->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetPosOnlyBuffer().GetAddressOf(), &stride, &offset);
		CONTEXT->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

		DrawIndexed(mesh->indexBuffer->GetCount());
	}
}
