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
	vector<SkeletalMeshVertexType> vertices;
	vector<uint32> indices;
	string materialName;
	int32 boneIndex;
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

struct PBRMaterial
{
	std::string name;

	Color albedoColor = Color(1.f, 1.f, 1.f, 1.f);
	std::string albedoFile{};
	std::string metallicRoughnessFile{};
	std::string normalFile{};
	std::string emissiveFile{};

	float metallicFactor = 1.0f;
	float roughnessFactor = 1.0f;
	
};
