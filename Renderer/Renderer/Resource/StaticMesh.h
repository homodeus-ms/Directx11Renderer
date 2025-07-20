#pragma once
#include "ResourceBase.h"
#include "Structs/ImportedMeshData.h"
#include "Structs/ShaderParameterTypes.h"

class StaticMesh : public ResourceBase
{
	using Super = ResourceBase;

public:
	StaticMesh();
	virtual ~StaticMesh();

	uint32 GetMaterialCount() { return static_cast<uint32>(_materials.size()); }
	vector<shared_ptr<Material>>& GetMaterials() { return _materials; }
	shared_ptr<Material> GetMaterialByIndex(uint32 index) { return _materials[index]; }
	shared_ptr<Material> GetMaterialByName(const wstring& name);
	void ChangeMaterialType(EMaterialType type);

	uint32 GetMeshCount() { return static_cast<uint32>(_meshes.size()); }
	vector<shared_ptr<ImportedStaticMesh>>& GetMeshes() { return _meshes; }
	shared_ptr<ImportedStaticMesh> GetMeshByIndex(uint32 index) { return _meshes[index]; }
	shared_ptr<ImportedStaticMesh> GetMeshByName(const wstring& name);

	uint32 GetBoneCount() { return static_cast<uint32>(_bones.size()); }
	vector<shared_ptr<ImportedStaticBone>>& GetBones() { return _bones; }
	shared_ptr<ImportedStaticBone> GetBoneByIndex(uint32 index) { return (index < 0 || index >= _bones.size() ? nullptr : _bones[index]); }
	shared_ptr<ImportedStaticBone> GetBoneByName(const wstring& name);

	shared_ptr<ImportedStaticBone>& GetBoneRoot() { return _root; }

	void BindStaticMeshElement();

private:
	shared_ptr<ImportedStaticBone> _root;
	vector<shared_ptr<Material>> _materials;
	vector<shared_ptr<ImportedStaticBone>> _bones;
	vector<shared_ptr<ImportedStaticMesh>> _meshes;
};

