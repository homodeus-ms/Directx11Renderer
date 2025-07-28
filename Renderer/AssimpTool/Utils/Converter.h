#pragma once
#include "ASTypes.h"

enum class MissingTextureFindKeyword
{
	Diffuse,
	Normal,
	Specular,
};

class Converter
{
public:
	Converter();
	~Converter();

public:
	void LoadRawAssetFile(wstring file);
	void ExportModelData(wstring savePath);
	void ExportMaterialData(wstring savePath);
	
	void ReadModelData(aiNode* node, int32 index, int32 parent);
	void ReadMeshData(aiNode* node, int32 bone);
	void ReadMaterialData(const wstring& pathForMissing);

	void WriteCustomMeshFile(wstring finalPath, bool bIsSkeletalMesh);
	void WriteCustomMaterialFile(wstring finalPath);
	string WriteTexture(string saveFolder, string file);

	void Cleanup();

public:
	void SetFoundTexturePath(string& setTarget, const string& path, const wstring& pathForMissing, MissingTextureFindKeyword keyword);
	string FindMissingTextureInFBXFolder(const wstring& findTargetPath, MissingTextureFindKeyword keyword);
	

	// obj, mtl
	void ReadObjFile(const wstring& path);
	void PrintMaterialInfo(const aiScene* scene);

	wstring _rawAssetPath = L"../Resources/RawAssets/";
	wstring _modelSaveRoot = L"../Resources/Assets/Model/";
	wstring _materialSaveRoot = L"../Resources/Assets/Material/";

	shared_ptr<Assimp::Importer> _importer;
	const aiScene* _scene;

	vector<shared_ptr<ASBone>> _bones;
	vector<shared_ptr<ASMesh>> _meshes;
	vector<shared_ptr<ASMaterial>> _materials;
};

