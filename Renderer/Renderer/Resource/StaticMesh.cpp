#include "pch.h"
#include "StaticMesh.h"
#include <filesystem>
#include "Utils/Utils.h"
#include "Utils/FileUtils.h"
#include "Utils/tinyxml2.h"
#include "Material/MaterialBase.h"
#include "Material/Material.h"

StaticMesh::StaticMesh(EResourceType type)
    : Super(type)
{
}

StaticMesh::~StaticMesh()
{
}

shared_ptr<MaterialBase> StaticMesh::GetMaterialByName(const wstring& name)
{
	for (auto& material : _materials)
	{
		if (material->GetName() == name)
			return material;
	}

	return nullptr;
}

void StaticMesh::ChangeMaterialType(EMaterialType type)
{
	for (auto& mat : _materials)
		mat->GetDesc()->MaterialType = type;
}

shared_ptr<ImportedMesh> StaticMesh::GetMeshByName(const wstring& name)
{
	for (auto& mesh : _meshes)
	{
		if (mesh->name == name)
			return mesh;
	}

	return nullptr;
}

shared_ptr<ImportedBone> StaticMesh::GetBoneByName(const wstring& name)
{
	for (auto& bone : _bones)
	{
		if (bone->name == name)
			return bone;
	}

	return nullptr;
}

void StaticMesh::BindMeshElements()
{
	// Mesh에 Material 캐싱
	for (const auto& mesh : _meshes)
	{
		// 이미 찾았으면 스킵
		if (mesh->material != nullptr)
			continue;

		mesh->material = GetMaterialByName(mesh->materialName);
	}
}

void StaticMesh::SetMeshes(const vector<shared_ptr<ImportedMesh>>& meshes)
{
	_meshes = meshes;
}

void StaticMesh::SetBones(const vector<shared_ptr<ImportedBone>>& bones)
{
	_bones = bones;
}

void StaticMesh::SetRoot(const shared_ptr<ImportedBone>& root)
{
	_root = root;
}

void StaticMesh::SetMaterials(const vector<shared_ptr<MaterialBase>>& materials)
{
	_materials = materials;
}
