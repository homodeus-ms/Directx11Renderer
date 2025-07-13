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
	void Cleanup();

public:
	void ReadModelDataFromAssimp(aiNode* node, int32 index, int32 parent);
	void ReadMeshDataFromAssimp(aiNode* node, int32 bone);
	void WriteCustomMeshFile(wstring finalPath);

	void ReadMaterialDataFromAssimp(const wstring& pathForMissing);
	void WriteCustomMaterialFile(wstring finalPath);
	string WriteTexture(string saveFolder, string file);

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

