#pragma once
#include "Graphics/Buffer/Geometry.h"

class VertexBuffer;
class IndexBuffer;
class Material;

struct ImportedStaticBone
{
	wstring name;
	int32 index;
	int32 parentIndex;
	shared_ptr<ImportedStaticBone> parent;

	Matrix transform;
	vector<shared_ptr<ImportedStaticBone>> children;
};

struct ImportedStaticMesh
{
	void CreateBuffers();

	wstring name{};

	// Mesh
	shared_ptr<Geometry<StaticMeshVertexType>> geometry = make_shared<Geometry<StaticMeshVertexType>>();
	shared_ptr<VertexBuffer> vertexBuffer;
	shared_ptr<IndexBuffer> indexBuffer;

	// Material
	wstring materialName = L"";
	shared_ptr<Material> material; // Cache

	// Bones
	int32 boneIndex;
	shared_ptr<ImportedStaticBone> bone; // Cache;
};