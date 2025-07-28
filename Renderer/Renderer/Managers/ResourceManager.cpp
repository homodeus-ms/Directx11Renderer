#include "pch.h"
#include "ResourceManager.h"
#include <filesystem>
#include "Utils/tinyxml2.h"
#include "Utils/Utils.h"
#include "Utils/FileUtils.h"
#include "Resource/Texture/LoadedTexture.h"
#include "Resource/BasicMesh/BasicMesh.h"
#include "Resource/BasicMesh/DefaultBasicMesh.h"
#include "Resource/BasicMesh/CubeMapBasicMesh.h"
#include "Resource/BasicMesh/VertexUVBasicMesh.h"
#include "Resource/Material.h"
#include "Resource/StaticMesh.h"
#include "Graphics/Shader/Shader.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "Utils/FileLoader.h"

void ResourceManager::BeginPlay()
{
	CreateDefaultMesh();
	CreateDefaultMaterials();
	_fileLoader = make_unique<FileLoader>();
}

void ResourceManager::CreateDefaultMesh()
{
	{
		shared_ptr<BasicMesh> mesh = make_shared<DefaultBasicMesh>();
		mesh->CreateQuad();
		Add(L"Quad", mesh);
	}
	{
		shared_ptr<BasicMesh> mesh = make_shared<DefaultBasicMesh>();
		mesh->CreateCube();
		Add(L"Cube", mesh);
	}
	{
		shared_ptr<BasicMesh> mesh = make_shared<DefaultBasicMesh>();
		mesh->CreateSphere();
		Add(L"Sphere", mesh);
	}
	{
		shared_ptr<BasicMesh> mesh = make_shared<CubeMapBasicMesh>();
		mesh->CreateCubeMap();
		Add(L"CubeMap", mesh);
	}
}

shared_ptr<StaticMesh> ResourceManager::LoadMeshFromAssetFolder(const wstring& key, const wstring& filename, bool bIsStaticMesh)
{
	if (shared_ptr<StaticMesh> get = Get<StaticMesh>(key))
		return get;

	assert(_fileLoader != nullptr);

	shared_ptr<StaticMesh> mesh = _fileLoader->LoadMeshOrNull(key, filename, bIsStaticMesh);

	if (mesh == nullptr)
	{
		LOG(Warning, "Load Mesh is Nullptr");
		assert(false);
	}

	return mesh;
}

shared_ptr<LoadedTexture> ResourceManager::GetOrAddTexture(const wstring& key, const wstring& path)
{
	if (filesystem::exists(filesystem::path(path)) == false)
		return nullptr;

	shared_ptr<LoadedTexture> texture = Load<LoadedTexture>(key, path);

	if (texture == nullptr)
	{
		LOG(Warning, "Texture Load Incompleted");
		texture = make_shared<LoadedTexture>();
		texture->Load(path);
		Add(key, texture);
	}

	return texture;
}

void ResourceManager::CreateDefaultMaterials()
{
	Load<LoadedTexture>(L"WhiteTexture", L"..\\Resources\\Images\\White.jpg");

	// Red
	{
		shared_ptr<Material> material = make_shared<Material>();
		material->SetDiffuseMap(Get<LoadedTexture>(L"WhiteTexture"));
		{
			MaterialDesc& desc = material->GetMaterialDesc();
			desc.ambient = Vec4(0.8f);
			desc.diffuse = Vec4(1.f, 0.f, 0.f, 1.f);
			desc.specular = Vec4(1.f);
			desc.emissive = Vec4(0.f, 0.f, 0.0f, 1.f);
			desc.bUnLit = 0;
		}
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"BasicMeshShader.hlsl");
		material->SetShaderInfo(shaderInfo);
		Add(L"BasicRed", material);
	}

	{
		shared_ptr<Material> material = make_shared<Material>();
		material->SetDiffuseMap(Get<LoadedTexture>(L"WhiteTexture"));
		{
			MaterialDesc& desc = material->GetMaterialDesc();
			desc.ambient = Vec4(0.8f);
			desc.diffuse = Vec4(1.f, 0.f, 0.f, 1.f);
			desc.specular = Vec4(1.f);
			desc.emissive = Vec4(0.f, 0.f, 0.0f, 1.f);
			desc.bUnLit = 0;
		}
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"BasicMeshShader.hlsl");
		material->SetShaderInfo(shaderInfo);
		Add(L"BasicRed", material);
	}
	// Blue
	{
		shared_ptr<Material> material = make_shared<Material>();
		material->SetDiffuseMap(Get<LoadedTexture>(L"WhiteTexture"));
		{
			MaterialDesc& desc = material->GetMaterialDesc();
			desc.ambient = Vec4(0.8f);
			desc.diffuse = Vec4(0.f, 0.f, 1.f, 1.f);
			desc.specular = Vec4(1.f);
			desc.emissive = Vec4(0.f, 0.f, 0.0f, 1.f);
			desc.bUnLit = 0;
		}
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"BasicMeshShader.hlsl");
		material->SetShaderInfo(shaderInfo);
		Add(L"BasicBlue", material);
	}

	// Green
	{
		shared_ptr<Material> material = make_shared<Material>();
		material->SetDiffuseMap(Get<LoadedTexture>(L"WhiteTexture"));
		{
			MaterialDesc& desc = material->GetMaterialDesc();
			desc.ambient = Vec4(0.8f);
			desc.diffuse = Vec4(0.f, 1.f, 0.f, 1.f);
			desc.specular = Vec4(1.f);
			desc.emissive = Vec4(0.f, 0.f, 0.0f, 1.f);
			desc.bUnLit = 0;
		}
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"BasicMeshShader.hlsl");
		material->SetShaderInfo(shaderInfo);
		Add(L"BasicGreen", material);
	}

	// Yellow
	{
		shared_ptr<Material> material = make_shared<Material>();
		material->SetDiffuseMap(Get<LoadedTexture>(L"WhiteTexture"));
		{
			MaterialDesc& desc = material->GetMaterialDesc();
			desc.ambient = Vec4(0.8f);
			desc.diffuse = Vec4(1.f, 1.f, 0.f, 1.f);
			desc.specular = Vec4(1.f);
			desc.emissive = Vec4(0.f, 0.f, 0.0f, 1.f);
			desc.bUnLit = 0;
		}
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"BasicMeshShader.hlsl");
		material->SetShaderInfo(shaderInfo);
		Add(L"BasicYellow", material);
	}

	// White
	{
		shared_ptr<Material> material = make_shared<Material>();
		material->SetDiffuseMap(Get<LoadedTexture>(L"WhiteTexture"));
		{
			MaterialDesc& desc = material->GetMaterialDesc();
			desc.ambient = Vec4(0.9f);
			desc.diffuse = Vec4(0.9f, 0.9f, 0.9f, 1.f);
			desc.specular = Vec4(1.f);
			desc.emissive = Vec4(0.f, 0.f, 0.0f, 1.f);
			desc.bUnLit = 0;
		}
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"BasicMeshShader.hlsl");
		material->SetShaderInfo(shaderInfo);
		Add(L"BasicWhite", material);
	}
}