#include "pch.h"
#include "StaticMesh.h"
#include <filesystem>
#include "Utils/Utils.h"
#include "Utils/FileUtils.h"
#include "Utils/tinyxml2.h"
#include "Material.h"

StaticMesh::StaticMesh()
	: Super(EResourceType::StaticMesh)
{
}

StaticMesh::~StaticMesh()
{
}

shared_ptr<Material> StaticMesh::GetMaterialByName(const wstring& name)
{
	for (auto& material : _materials)
	{
		if (material->GetName() == name)
			return material;
	}

	return nullptr;
}

shared_ptr<ImportedStaticMesh> StaticMesh::GetMeshByName(const wstring& name)
{
	for (auto& mesh : _meshes)
	{
		if (mesh->name == name)
			return mesh;
	}

	return nullptr;
}

shared_ptr<ImportedStaticBone> StaticMesh::GetBoneByName(const wstring& name)
{
	for (auto& bone : _bones)
	{
		if (bone->name == name)
			return bone;
	}

	return nullptr;
}

void StaticMesh::BindStaticMeshElement()
{
	// Mesh에 Material 캐싱
	for (const auto& mesh : _meshes)
	{
		// 이미 찾았으면 스킵
		if (mesh->material != nullptr)
			continue;

		mesh->material = GetMaterialByName(mesh->materialName);
	}

	// Mesh에 Bone 캐싱
	for (const auto& mesh : _meshes)
	{
		// 이미 찾았으면 스킵
		if (mesh->bone != nullptr)
			continue;

		mesh->bone = GetBoneByIndex(mesh->boneIndex);
	}

	// Bone 계층 정보 채우기
	if (_root == nullptr && _bones.size() > 0)
	{
		_root = _bones[0];

		for (const auto& bone : _bones)
		{
			if (bone->parentIndex >= 0)
			{
				bone->parent = _bones[bone->parentIndex];
				bone->parent->children.push_back(bone);
			}
			else
			{
				bone->parent = nullptr;
			}
		}
	}
}

