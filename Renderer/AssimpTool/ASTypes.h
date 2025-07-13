#pragma once
#include "Graphics/Buffer/VertexData.h"

struct ASBone
{
	string name;
	int32 index = -1;
	int32 parent = -1;
	Matrix transform;
};

struct ASMesh
{
	string name;
	aiMesh* mesh;
	vector<StaticMeshVertexType> vertices;
	vector<uint32> indices;

	int32 boneIndex;
	string materialName;
};

struct ASMaterial
{
	string name;
	Color ambient;
	Color diffuse;
	Color specular;
	Color emissive;
	string diffuseFile;
	string specularFile;
	string normalFile;
};

