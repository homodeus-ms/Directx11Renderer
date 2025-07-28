#pragma once
#include "Graphics/Buffer/Geometry.h"

class VertexBuffer;
class IndexBuffer;
class Material;

struct ImportedBone
{
	wstring name;
	int32 index;
	int32 parentIndex;
	shared_ptr<ImportedBone> parent;

	Matrix transform;
	vector<shared_ptr<ImportedBone>> children;
};

struct ImportedMesh
{
	void CreateBuffers();

	wstring name{};

	// Mesh
	shared_ptr<Geometry<SkeletalMeshVertexType>> geometry = make_shared<Geometry<SkeletalMeshVertexType>>();
	shared_ptr<VertexBuffer> vertexBuffer;
	shared_ptr<IndexBuffer> indexBuffer;

	// Material
	wstring materialName = L"";
	shared_ptr<Material> material; // Cache

	// Bones
	int32 boneIndex;
	shared_ptr<ImportedBone> bone; // Cache;
};