#pragma once

#include "Resource/ResourceBase.h"
#include "Resource/BasicMesh.h"
#include "Structs/ImportedMeshData.h"
#include "Structs/ETextureType.h"

class Shader;
class Texture;
class Material;
class StaticMesh;

namespace tinyxml2
{
	class XMLElement;
}

class ResourceManager
{
	DECLARE_SINGLE(ResourceManager); 

public:
	void BeginPlay();

	template<typename T>
	shared_ptr<T> Load(const wstring& key, const wstring& path);

	// Can load file in a default path only
	// default path : ../Resource/Assets/
	shared_ptr<StaticMesh> LoadStaticMeshFromXML(const wstring& key, const wstring& filenameInAssetFolderOnly);

	template<typename T>
	bool Add(const wstring& key, shared_ptr<T> object);

	template<typename T>
	shared_ptr<T> Get(const wstring& key);

	template<typename T>
	EResourceType GetResourceType();

private:
	void CreateDefaultMesh();

	// Read Data From XML(Saved fbx Data) 
	bool GetFullPathFromAssetFolderByFileName(const wstring& filename, wstring& OUT meshPath, wstring& OUT materialPath);
	void ReadMaterialsFromXML(const wstring& filepath, vector<shared_ptr<Material>>& OUT materials);
	void ReadMeshesFromXML(const wstring& filepath, vector<shared_ptr<ImportedStaticBone>>& OUT bones, vector<shared_ptr<ImportedStaticMesh>>& OUT meshes);
	shared_ptr<Texture> GetOrAddTexture(const wstring& key, const wstring& path);
	void SetTextureToMaterial(const char* keyname, const wstring& parentPath, shared_ptr<Material> material, ETextureType textureType);
	Color ReadColorInfo(tinyxml2::XMLElement* node);

	
	wstring _resourcePath;
	wstring _assetPath = L"../Resources/Assets/";

	using KeyObjMap = map<wstring /*key*/, shared_ptr<ResourceBase>>;
	array<KeyObjMap, RESOURCE_TYPE_COUNT> _resources;
};

template<typename T>
EResourceType ResourceManager::GetResourceType()
{
	if (std::is_same_v<T, Texture>)
		return EResourceType::Texture;
	if (std::is_base_of_v<BasicMesh, T>)
		return EResourceType::Mesh;
	if (std::is_same_v<T, Material>)
		return EResourceType::Material;
	if (std::is_same_v<T, Shader>)
		return EResourceType::Shader;
	if (std::is_same_v<T, StaticMesh>)
		return EResourceType::StaticMesh;
	/*if (std::is_same_v<T, SkeletalMesh>)
		return EResourceType::SkeletalMesh;
	if (std::is_same_v<T, Animation>)
		return ResourceType::Animation;*/
	

	assert(false);
	return EResourceType::None;
}

template<typename T>
shared_ptr<T> ResourceManager::Load(const wstring& key, const wstring& path)
{
	auto objectType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	shared_ptr<T> object = make_shared<T>();
	object->Load(path);
	keyObjMap[key] = object;

	return object;
}

template<typename T>
bool ResourceManager::Add(const wstring& key, shared_ptr<T> object)
{
	EResourceType resourceType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return false;

	keyObjMap[key] = object;
	return true;
}

template<typename T>
shared_ptr<T> ResourceManager::Get(const wstring& key)
{
	EResourceType resourceType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	return nullptr;
}

