#pragma once
#include "MeshBase.h"
#include "Structs/ImportedMeshData.h"
#include "Structs/ShaderParameterTypes.h"

class StaticMesh : public MeshBase
{
	using Super = MeshBase;

public:
	StaticMesh(EResourceType type = EResourceType::StaticMesh);
	virtual ~StaticMesh();

	uint32 GetMaterialCount() { return static_cast<uint32>(_materials.size()); }
	vector<shared_ptr<MaterialBase>>& GetMaterials() { return _materials; }
	shared_ptr<MaterialBase> GetMaterialByIndex(uint32 index) { return _materials[index]; }
	shared_ptr<MaterialBase> GetMaterialByName(const wstring& name);
	void ChangeMaterialType(EMaterialType type);

	uint32 GetMeshCount() { return static_cast<uint32>(_meshes.size()); }
	vector<shared_ptr<ImportedMesh>>& GetMeshes() { return _meshes; }
	shared_ptr<ImportedMesh> GetMeshByIndex(uint32 index) { return _meshes[index]; }
	shared_ptr<ImportedMesh> GetMeshByName(const wstring& name);

	uint32 GetBoneCount() { return static_cast<uint32>(_bones.size()); }
	vector<shared_ptr<ImportedBone>>& GetBones() { return _bones; }
	shared_ptr<ImportedBone> GetBoneByIndex(uint32 index) { return (index < 0 || index >= _bones.size() ? nullptr : _bones[index]); }
	shared_ptr<ImportedBone> GetBoneByName(const wstring& name);
	shared_ptr<ImportedBone>& GetBoneRoot() { return _root; }

	virtual void BindMeshElements();


	void SetMeshes(const vector<shared_ptr<ImportedMesh>>& meshes);
	void SetBones(const vector<shared_ptr<ImportedBone>>& bones);
	void SetRoot(const shared_ptr<ImportedBone>& root);
	void SetMaterials(const vector<shared_ptr<MaterialBase>>& materials);

protected:
	vector<shared_ptr<MaterialBase>> _materials{};
	vector<shared_ptr<ImportedMesh>> _meshes{};
	shared_ptr<ImportedBone> _root{};
	vector<shared_ptr<ImportedBone>> _bones{};
};

