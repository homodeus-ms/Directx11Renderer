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

void ResourceManager::BeginPlay()
{
	CreateDefaultMesh();
	CreateDefaultMaterials();
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

shared_ptr<StaticMesh> ResourceManager::LoadStaticMeshFromXML(const wstring& key, const wstring& filename)
{
	if (shared_ptr<StaticMesh> get = Get<StaticMesh>(key))
		return get;

	shared_ptr<StaticMesh> staticMesh = make_shared<StaticMesh>();

	wstring meshPath = L"";
	wstring materialPath = L"";

	bool bFoundBoth = GetFullPathFromAssetFolderByFileName(filename, meshPath, materialPath);
	if (!bFoundBoth)
	{
		if (meshPath == L"")
			LOG(Warning, "Static Mesh's meshPath Not Found");
		else
			LOG(Warning, "Static Mesh' materialPath Not Found");
	}
	
	// Found Both Paths
	ReadMeshesFromXML(meshPath, staticMesh->GetBones(), staticMesh->GetMeshes());
	ReadMaterialsFromXML(materialPath, staticMesh->GetMaterials());
	
	staticMesh->BindStaticMeshElement();

	Add(key, staticMesh);

	return staticMesh;
}


bool ResourceManager::GetFullPathFromAssetFolderByFileName(const wstring& filename, wstring& OUT meshPath, wstring& OUT materialPath)
{
	bool foundMesh = false;
	bool foundMaterial = false;

	wstring meshTarget = filename + L".mesh";
	wstring materialTarget = filename + L".xml";

	for (const auto& entry : std::filesystem::recursive_directory_iterator(_assetPath))
	{
		if (!entry.is_regular_file())
			continue;

		const std::filesystem::path& path = entry.path();
		const wstring& currentFilename = path.filename().wstring();

		if (!foundMesh && currentFilename == meshTarget)
		{
			meshPath = path.wstring();
			Utils::Replace(meshPath, L"\\", L"/");
			foundMesh = true;
		}
		else if (!foundMaterial && currentFilename == materialTarget)
		{
			materialPath = path.wstring();
			Utils::Replace(materialPath, L"\\", L"/");
			foundMaterial = true;
		}

		// 둘 다 찾았으면 조기 종료
		if (foundMesh && foundMaterial)
			break;
	}

	return (foundMesh || foundMaterial);
}

void ResourceManager::ReadMaterialsFromXML(const wstring& filepath, vector<shared_ptr<Material>>& OUT materials)
{
	string logMsg = "ReadMaterial from " + Utils::ToString(filepath);
	LOG(Log, logMsg);

	auto parentPath = std::filesystem::path(filepath).parent_path();
	wstring parentPathStr = parentPath.wstring();

	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	tinyxml2::XMLError error = document->LoadFile(Utils::ToString(filepath).c_str());
	assert(error == tinyxml2::XML_SUCCESS);

	tinyxml2::XMLElement* root = document->FirstChildElement();
	tinyxml2::XMLElement* materialNode = root->FirstChildElement();

	while (materialNode)
	{
		shared_ptr<Material> material = make_shared<Material>();

		tinyxml2::XMLElement* node = nullptr;

		node = materialNode->FirstChildElement();
		material->SetName(Utils::ToWString(node->GetText()));

		// Diffuse Texture
		node = node->NextSiblingElement();
		if (const char* textureName = node->GetText())
			SetTextureToMaterial(textureName, parentPathStr, material, ETextureType::Diffuse);
		
		// Specular Texture
		node = node->NextSiblingElement();
		if (const char* textureName = node->GetText())
			SetTextureToMaterial(textureName, parentPathStr, material, ETextureType::Specular);


		// Normal Texture
		node = node->NextSiblingElement();
		if (const char* textureName = node->GetText())
			SetTextureToMaterial(textureName, parentPathStr, material, ETextureType::Normal);


		// Ambient
		{
			node = node->NextSiblingElement();
			const Color& color = ReadColorInfo(node);
			material->GetMaterialDesc().ambient = color;
		}

		// Diffuse
		{
			node = node->NextSiblingElement();
			const Color& color = ReadColorInfo(node);
			material->GetMaterialDesc().diffuse = color;
		}

		// Specular
		{
			node = node->NextSiblingElement();
			const Color& color = ReadColorInfo(node);
			material->GetMaterialDesc().specular = color;
		}

		// Emissive
		{
			node = node->NextSiblingElement();
			const Color& color = ReadColorInfo(node);
			material->GetMaterialDesc().emissive = color;
		}

		materials.push_back(material);

		// Next Material
		materialNode = materialNode->NextSiblingElement();
	}
}

void ResourceManager::ReadMeshesFromXML(const wstring& filepath, vector<shared_ptr<ImportedStaticBone>>& OUT bones, vector<shared_ptr<ImportedStaticMesh>>& OUT meshes)
{
	string logMsg = "ReadMesh from " + Utils::ToString(filepath);
	LOG(Log, logMsg);

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(filepath, FileMode::Read);

	// Bones
	{
		const uint32 count = file->Read<uint32>();

		for (uint32 i = 0; i < count; i++)
		{
			shared_ptr<ImportedStaticBone> bone = make_shared<ImportedStaticBone>();
			bone->index = file->Read<int32>();
			bone->name = Utils::ToWString(file->Read<string>());
			bone->parentIndex = file->Read<int32>();
			bone->transform = file->Read<Matrix>();

			bones.push_back(bone);
		}
	}

	// Mesh
	{
		const uint32 count = file->Read<uint32>();

		for (uint32 i = 0; i < count; i++)
		{
			shared_ptr<ImportedStaticMesh> mesh = make_shared<ImportedStaticMesh>();

			mesh->name = Utils::ToWString(file->Read<string>());
			mesh->boneIndex = file->Read<int32>();

			// Material
			mesh->materialName = Utils::ToWString(file->Read<string>());

			//VertexData
			{
				const uint32 count = file->Read<uint32>();
				vector<StaticMeshVertexType> vertices;
				vertices.resize(count);

				void* data = vertices.data();
				file->Read(&data, sizeof(StaticMeshVertexType) * count);
				mesh->geometry->AddVertices(vertices);
			}

			//IndexData
			{
				const uint32 count = file->Read<uint32>();

				vector<uint32> indices;
				indices.resize(count);

				void* data = indices.data();
				file->Read(&data, sizeof(uint32) * count);
				mesh->geometry->AddIndices(indices);
			}

			mesh->CreateBuffers();

			meshes.push_back(mesh);
		}
	}
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

void ResourceManager::SetTextureToMaterial(const char* keyname, const wstring& parentPath, shared_ptr<Material> material, ETextureType textureType)
{
	if (!keyname)
		return;
	
	wstring fullpath = parentPath + L"/" + Utils::ToWString(keyname);    // TEMP
	string keynameWithoutExt = Utils::GetFilenameWithoutExt(string(keyname));
	wstring key = Utils::ToWString(keynameWithoutExt);

	if (!key.empty())
	{
		shared_ptr<LoadedTexture> texture = GetOrAddTexture(key, fullpath);
		material->SetTextureMap(textureType, texture);
	}
}

Color ResourceManager::ReadColorInfo(tinyxml2::XMLElement* node)
{
	Color color;
	color.x = node->FloatAttribute("R");
	color.y = node->FloatAttribute("G");
	color.z = node->FloatAttribute("B");
	color.w = node->FloatAttribute("A");

	return color;
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