#pragma once
#include "Structs/ETextureType.h"
#include "Structs/ImportedMeshData.h"
#include "tinyxml2.h"

class StaticMesh;
class SkeletalMesh;
class Material;

class FileLoader
{
public:
	shared_ptr<StaticMesh> LoadMeshOrNull(const wstring& key, const wstring& filenameInAssetFolder, bool bIsStaticMesh);
	
private:
	const wstring ASSET_PATH = L"../Resources/Assets/";

	void ReadMeshData(const wstring& filepath);
	void ReadMaterialData(const wstring& filepath);

	shared_ptr<StaticMesh> CreateAndBindStaticMesh();
	shared_ptr<SkeletalMesh> CreateAndBindSkeletalMesh();


	// Helper Funcs
	uint32 GetMaterialCount() { return static_cast<uint32>(_materials.size()); }
	vector<shared_ptr<Material>>& GetMaterials() { return _materials; }
	shared_ptr<Material> GetMaterialByIndex(uint32 index) { return _materials[index]; }
	shared_ptr<Material> GetMaterialByName(const wstring& name);

	uint32 GetMeshCount() { return static_cast<uint32>(_meshes.size()); }
	vector<shared_ptr<ImportedMesh>>& GetMeshes() { return _meshes; }
	shared_ptr<ImportedMesh> GetMeshByIndex(uint32 index) { return _meshes[index]; }
	shared_ptr<ImportedMesh> GetMeshByName(const wstring& name);

	uint32 GetBoneCount() { return static_cast<uint32>(_bones.size()); }
	vector<shared_ptr<ImportedBone>>& GetBones() { return _bones; }
	shared_ptr<ImportedBone> GetBoneByIndex(uint32 index) { return (index < 0 || index >= _bones.size() ? nullptr : _bones[index]); }
	shared_ptr<ImportedBone> GetBoneByName(const wstring& name);
	//shared_ptr<ImportedBone>& GetBoneRoot() { return _root; }

	void NormalizeVectices();
	void CreateEachMeshBuffers();

	void SetTextureToMaterial(const char* keyname, const wstring& parentPath, shared_ptr<Material> material, ETextureType textureType);
	Color ReadColorInfo(tinyxml2::XMLElement* node);

	bool GetFullPathByFileNameRecursive(const wstring& filename, wstring& OUT meshPath, wstring& OUT materialPath);
	void CleanUp();


	shared_ptr<ImportedBone> _root;
	vector<shared_ptr<ImportedBone>> _bones;
	vector<shared_ptr<ImportedMesh>> _meshes;
	vector<shared_ptr<Material>> _materials;
};

