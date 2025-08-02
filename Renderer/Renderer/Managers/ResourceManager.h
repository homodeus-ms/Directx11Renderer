#pragma once

#include "Resource/ResourceBase.h"
#include "Resource/BasicMesh/BasicMesh.h"
#include "Utils/FileLoader.h"

class Shader;
class LoadedTexture;
class Material;
class StaticMesh;
class LUT;

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

	shared_ptr<StaticMesh> LoadMeshFromAssetFolder(const wstring& key, const wstring& filename, bool bIsStaticMesh = true);

	template<typename T>
	bool Add(const wstring& key, shared_ptr<T> object);

	template<typename T>
	shared_ptr<T> Get(const wstring& key);

	template<typename T>
	EResourceType GetResourceType();

	shared_ptr<LoadedTexture> GetOrAddTexture(const wstring& key, const wstring& path);

private:
	void CreateDefaultMesh();
	void CreateDefaultMaterials();
	
	wstring _resourcePath;
	wstring _assetPath = L"../Resources/Assets/";

	using KeyObjMap = map<wstring /*key*/, shared_ptr<ResourceBase>>;
	array<KeyObjMap, RESOURCE_TYPE_COUNT> _resources;

	std::unique_ptr<FileLoader> _fileLoader;
};

template<typename T>
EResourceType ResourceManager::GetResourceType()
{
	if (std::is_same_v<T, LoadedTexture>)
		return EResourceType::Texture;
	if (std::is_base_of_v<BasicMesh, T>)
		return EResourceType::BasicMesh;
	if (std::is_same_v<T, Material>)
		return EResourceType::Material;
	if (std::is_same_v<T, Shader>)
		return EResourceType::Shader;
	if (std::is_same_v<T, StaticMesh>)
		return EResourceType::StaticMesh;
	if (std::is_same_v<T, LUT>)
		return EResourceType::LUT;

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

