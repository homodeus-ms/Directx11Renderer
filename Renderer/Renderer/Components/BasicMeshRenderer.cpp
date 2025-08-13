#include "pch.h"
#include "BasicMeshRenderer.h"
#include "Resource/BasicMesh/BasicMesh.h"
#include "Resource/Material/MaterialBase.h"
#include "Resource/Material/Material.h"
#include "Resource/Texture/LoadedTexture.h"
#include "Actor/Actor.h"
#include "Components/Transform.h"
#include "Components/CameraComponent.h"
#include "Graphics/Buffer/InputLayout.h"
#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Buffer/IndexBuffer.h"
#include "Graphics/Shader/VertexShader.h"
#include "Graphics/Shader/PixelShader.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "Graphics/PipelineState/PipelineState.h"
#include "Managers/ShaderParameterManager.h"
#include "Structs/ShaderParameterTypes.h"

BasicMeshRenderer::BasicMeshRenderer()
	: Super(EComponentType::BasicMeshRenderer)
{
}

BasicMeshRenderer::~BasicMeshRenderer()
{
}

void BasicMeshRenderer::Construct()
{
	_bRenderReady = _basicMesh != nullptr && _basicMaterial != nullptr;
}

void BasicMeshRenderer::BeginPlay()
{
	Super::BeginPlay();
}

void BasicMeshRenderer::Render()
{
	Super::Render();

	auto world = GetOwnerTransform()->GetWorldMatrix();
	auto invWorld = GetOwnerTransform()->GetInvWorldMatrix();
	SHADER_PARAM_MANAGER->PushTransformData(TransformDesc{ world, invWorld });

	if (_basicMaterial != nullptr && _basicMaterial->GetDesc()->MaterialType != EMaterialType::EnvMap)
	{
		SHADER_PARAM_MANAGER->PushMaterial(_basicMaterial);
	}

	SHADER_PARAM_MANAGER->BindAllDirtyBuffers();

	uint32 stride = _basicMesh->GetVertexBuffer()->GetStride();
	uint32 offset = _basicMesh->GetVertexBuffer()->GetOffset();

	CONTEXT->IASetVertexBuffers(0, 1, _basicMesh->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
	CONTEXT->IASetIndexBuffer(_basicMesh->GetIndexBuffer()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	DrawIndexed(_basicMesh->GetIndexBuffer()->GetCount());
}

void BasicMeshRenderer::RenderDepthOnly(bool bForPointLight, int32 instanceCount)
{
	Super::RenderDepthOnly(bForPointLight, instanceCount);

	auto world = GetOwnerTransform()->GetWorldMatrix();
	auto invWorld = GetOwnerTransform()->GetInvWorldMatrix();
	SHADER_PARAM_MANAGER->PushTransformData(TransformDesc{ world, invWorld });
	SHADER_PARAM_MANAGER->BindAllDirtyBuffers();

	uint32 stride = sizeof(VertexData);
	uint32 offset = _basicMesh->GetVertexBuffer()->GetOffset();
	CONTEXT->IASetVertexBuffers(0, 1, _basicMesh->GetVertexBuffer()->GetPosOnlyBuffer().GetAddressOf(), &stride, &offset);
	CONTEXT->IASetIndexBuffer(_basicMesh->GetIndexBuffer()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	//if (!bForPointLight)
	{
		DrawIndexed(_basicMesh->GetIndexBuffer()->GetCount());
	}
	//else
	//{
	//	assert(instanceCount <= MAX_SHADOW_MAP_COUNT);
	//	DrawIndexedInstanced(_basicMesh->GetIndexBuffer()->GetCount(), instanceCount);
	//}

	//ClearGeometryShader();
}

void BasicMeshRenderer::RenderDrawNormal()
{
	Super::RenderDrawNormal();

	auto world = GetOwnerTransform()->GetWorldMatrix();
	auto invWorld = GetOwnerTransform()->GetInvWorldMatrix();
	SHADER_PARAM_MANAGER->PushTransformData(TransformDesc{ world, invWorld });
	SHADER_PARAM_MANAGER->BindAllDirtyBuffers();

	uint32 stride = _basicMesh->GetVertexBuffer()->GetStride();
	uint32 offset = _basicMesh->GetVertexBuffer()->GetOffset();
	CONTEXT->IASetVertexBuffers(0, 1, _basicMesh->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
	
	Draw(_basicMesh->GetVertexBuffer()->GetCount(), 0);
}

vector<shared_ptr<MaterialBase>> BasicMeshRenderer::GetMaterials()
{
	vector<shared_ptr<MaterialBase>> materials;
	materials.push_back(_basicMaterial);
	return materials;
}

void BasicMeshRenderer::SetBasicMesh(const shared_ptr<BasicMesh>& mesh)
{
	_basicMesh = mesh;
	_bRenderReady = _basicMesh != nullptr && _basicMaterial != nullptr;
}

void BasicMeshRenderer::SetBasicMaterial(const shared_ptr<MaterialBase>& material)
{
	_basicMaterial = material;
	//SetVertexShader(_basicMaterial->GetShaderInfo());
	//SetPixelShader(_basicMaterial->GetShaderInfo());
	_bRenderReady = _basicMesh != nullptr && _basicMaterial != nullptr;
}

void BasicMeshRenderer::ChangeMaterialType(EMaterialType type)
{
	_basicMaterial->GetDesc()->MaterialType = type;
}


