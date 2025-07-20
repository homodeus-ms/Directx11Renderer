#include "pch.h"
#include "BasicMeshRenderer.h"
#include "Resource/BasicMesh.h"
#include "Resource/Material.h"
#include "Resource/Texture.h"
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
	SHADER_PARAM_MANAGER->PushTransformData(TransformDesc{ world });

	if (_basicMaterial != nullptr)
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

void BasicMeshRenderer::SetInputLayout()
{
	const vector<D3D11_INPUT_ELEMENT_DESC>& desc = _basicMesh->GetInputLayoutDesc();
	_inputLayout = make_shared<InputLayout>();
	_inputLayout->Create(desc, _vertexShader->GetBlob());
}

vector<shared_ptr<Material>> BasicMeshRenderer::GetMaterials()
{
	vector<shared_ptr<Material>> materials;
	materials.push_back(_basicMaterial);
	return materials;
}

void BasicMeshRenderer::SetBasicMesh(const shared_ptr<BasicMesh>& mesh)
{
	_basicMesh = mesh;
	_bRenderReady = _basicMesh != nullptr && _basicMaterial != nullptr;
}

void BasicMeshRenderer::SetBasicMaterial(const shared_ptr<Material>& material)
{
	_basicMaterial = material;
	SetVertexShader(_basicMaterial->GetShaderInfo());
	SetPixelShader(_basicMaterial->GetShaderInfo());
	_bRenderReady = _basicMesh != nullptr && _basicMaterial != nullptr;
}

void BasicMeshRenderer::ChangeMaterialType(EMaterialType type)
{
	_basicMaterial->GetDesc()->MaterialType = type;
}


