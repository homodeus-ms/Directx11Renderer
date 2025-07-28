#include "pch.h"
#include "SkeletalMesh.h"

SkeletalMesh::SkeletalMesh()
    : Super(EResourceType::SkeletalMesh)
{
}

SkeletalMesh::~SkeletalMesh()
{
}

void SkeletalMesh::BindMeshElements()
{
	Super::BindMeshElements();

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
