#include "pch.h"
#include "FileLoader.h"
#include <filesystem>
#include "Utils.h"
#include "FileUtils.h"
#include "tinyxml2.h"
#include "Resource/Material.h"
#include "Resource/ResourceBase.h"
#include "Resource/StaticMesh.h"
#include "Resource/SkeletalMesh.h"

shared_ptr<StaticMesh> FileLoader::LoadMeshOrNull(const wstring& key, const wstring& filename, bool bIsStaticMesh)
{
	if (shared_ptr<StaticMesh> exist = RESOURCE_MANAGER->Get<StaticMesh>(key))
		return exist;

	wstring meshPath = L"";
	wstring materialPath = L"";

	bool bFoundBoth = GetFullPathByFileNameRecursive(filename, meshPath, materialPath);
	if (!bFoundBoth)
		return nullptr;

	ReadMeshData(meshPath);
	ReadMaterialData(materialPath);

	shared_ptr<StaticMesh> mesh = nullptr;
	if (bIsStaticMesh)
	{
		mesh = CreateAndBindStaticMesh();
	}
	else
	{
		mesh = CreateAndBindSkeletalMesh();
	}

	RESOURCE_MANAGER->Add(key, mesh);
	CleanUp();
	return mesh;
}

void FileLoader::ReadMeshData(const wstring& filepath)
{
	string logMsg = "ReadMesh from " + Utils::ToString(filepath);
	LOG(Log, logMsg);

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(filepath, FileMode::Read);

	// Bones
	{
		const uint32 boneCount = file->Read<uint32>();
		_bones.reserve(boneCount);

		for (uint32 i = 0; i < boneCount; i++)
		{
			shared_ptr<ImportedBone> bone = make_shared<ImportedBone>();
			bone->index = file->Read<int32>();
			bone->name = Utils::ToWString(file->Read<string>());
			bone->parentIndex = file->Read<int32>();
			bone->transform = file->Read<Matrix>();

			_bones.push_back(bone);
		}
	}

	// Mesh
	{
		const uint32 meshCount = file->Read<uint32>();

		for (uint32 i = 0; i < meshCount; i++)
		{
			shared_ptr<ImportedMesh> mesh = make_shared<ImportedMesh>();

			mesh->name = Utils::ToWString(file->Read<string>());
			mesh->boneIndex = file->Read<int32>();

			// Material
			mesh->materialName = Utils::ToWString(file->Read<string>());

			//VertexData
			{
				const uint32 count = file->Read<uint32>();
				vector<SkeletalMeshVertexType> vertices;
				vertices.resize(count);

				void* data = vertices.data();
				file->Read(&data, sizeof(SkeletalMeshVertexType) * count);
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

			_meshes.push_back(mesh);
		}
	}
}

void FileLoader::ReadMaterialData(const wstring& filepath)
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

		_materials.push_back(material);

		// Next Material
		materialNode = materialNode->NextSiblingElement();
	}
}

shared_ptr<StaticMesh> FileLoader::CreateAndBindStaticMesh()
{
	shared_ptr<StaticMesh> staticMesh = make_shared<StaticMesh>();

	for (const auto& mesh : _meshes)
	{
		// 이미 찾았으면 스킵
		if (mesh->bone != nullptr)
			continue;

		mesh->bone = GetBoneByIndex(mesh->boneIndex);
	}

	// Mesh에 Material 캐싱
	for (const shared_ptr<ImportedMesh>& mesh : _meshes)
	{
		vector<SkeletalMeshVertexType>& vertices = mesh->geometry->GetVertices();

		for (auto& vertex : vertices)
		{
			const Matrix& toRoot = mesh->bone->transform;
			vertex.position = DirectX::SimpleMath::Vector3::Transform(vertex.position, toRoot);
		}

		//NormalizeVectices(vertices);
		//mesh->CreateBuffers();

		// 이미 찾았으면 스킵
		if (mesh->material != nullptr)
			continue;

		mesh->material = GetMaterialByName(mesh->materialName);

	}

	NormalizeVectices();
	CreateEachMeshBuffers();

	staticMesh->SetMeshes(_meshes);
	staticMesh->SetMaterials(_materials);

	return staticMesh;
}

shared_ptr<SkeletalMesh> FileLoader::CreateAndBindSkeletalMesh()
{
	shared_ptr<SkeletalMesh> mesh = make_shared<SkeletalMesh>();

	return mesh;
}

// Helper Funcs

shared_ptr<Material> FileLoader::GetMaterialByName(const wstring& name)
{
	for (auto& material : _materials)
	{
		if (material->GetName() == name)
			return material;
	}

	return nullptr;
}

shared_ptr<ImportedMesh> FileLoader::GetMeshByName(const wstring& name)
{
	for (auto& mesh : _meshes)
	{
		if (mesh->name == name)
			return mesh;
	}

	return nullptr;
}

shared_ptr<ImportedBone> FileLoader::GetBoneByName(const wstring& name)
{
	for (auto& bone : _bones)
	{
		if (bone->name == name)
			return bone;
	}

	return nullptr;
}

void FileLoader::NormalizeVectices()
{
	Vec3 vmin(1000, 1000, 1000);
	Vec3 vmax(-1000, -1000, -1000);

	for (shared_ptr<ImportedMesh>& mesh : _meshes) 
	{
		vector<SkeletalMeshVertexType>& veritces = mesh->geometry->GetVertices();

		for (SkeletalMeshVertexType& v : veritces) 
		{
			vmin.x = XMMin(vmin.x, v.position.x);
			vmin.y = XMMin(vmin.y, v.position.y);
			vmin.z = XMMin(vmin.z, v.position.z);
			vmax.x = XMMax(vmax.x, v.position.x);
			vmax.y = XMMax(vmax.y, v.position.y);
			vmax.z = XMMax(vmax.z, v.position.z);
		}
	}

	float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
	float dl = XMMax(XMMax(dx, dy), dz);
	float cx = (vmax.x + vmin.x) * 0.5f, cy = (vmax.y + vmin.y) * 0.5f,
		cz = (vmax.z + vmin.z) * 0.5f;

	for (shared_ptr<ImportedMesh>& mesh : _meshes) 
	{
		vector<SkeletalMeshVertexType>& veritces = mesh->geometry->GetVertices();

		for (SkeletalMeshVertexType& v : veritces) 
		{
			v.position.x = (v.position.x - cx) / dl;
			v.position.y = (v.position.y - cy) / dl;
			v.position.z = (v.position.z - cz) / dl;
		}
	}
}

void FileLoader::CreateEachMeshBuffers()
{
	for (shared_ptr<ImportedMesh>& mesh : _meshes)
	{
		mesh->CreateBuffers();
	}
}

void FileLoader::SetTextureToMaterial(const char* keyname, const wstring& parentPath, shared_ptr<Material> material, ETextureType textureType)
{
	if (!keyname)
		return;

	wstring fullpath = parentPath + L"/" + Utils::ToWString(keyname);    // TEMP
	string keynameWithoutExt = Utils::GetFilenameWithoutExt(string(keyname));
	wstring key = Utils::ToWString(keynameWithoutExt);

	if (!key.empty())
	{
		shared_ptr<LoadedTexture> texture = RESOURCE_MANAGER->GetOrAddTexture(key, fullpath);
		material->SetTextureMap(textureType, texture);
	}
}

Color FileLoader::ReadColorInfo(tinyxml2::XMLElement* node)
{
	Color color;
	color.x = node->FloatAttribute("R");
	color.y = node->FloatAttribute("G");
	color.z = node->FloatAttribute("B");
	color.w = node->FloatAttribute("A");

	return color;
}

bool FileLoader::GetFullPathByFileNameRecursive(const wstring& filename, wstring& OUT meshPath, wstring& OUT materialPath)
{
	bool foundMesh = false;
	bool foundMaterial = false;

	wstring meshTarget = filename + L".mesh";
	wstring materialTarget = filename + L".xml";

	for (const auto& entry : std::filesystem::recursive_directory_iterator(ASSET_PATH))
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
		
		if (foundMesh && foundMaterial)
			break;
	}

	if (foundMesh && foundMaterial)
		return true;
	
	// LOG
	if (!foundMesh)
		LOG(Warning, "Static Mesh's meshPath Not Found");
	else
		LOG(Warning, "Static Mesh' materialPath Not Found");

	return false;
}

void FileLoader::CleanUp()
{
	_bones.clear();
	_meshes.clear();
	_materials.clear();
}
