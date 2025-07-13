#include "pch.h"
#include "Converter.h"
#include <filesystem>
#include "Utils/Utils.h"
#include "Utils/tinyxml2.h"
#include "Utils/FileUtils.h"

Converter::Converter()
{
	_importer = make_shared<Assimp::Importer>();
}

Converter::~Converter()
{
}

void Converter::LoadRawAssetFile(wstring file)
{
	wstring fileStr = _rawAssetPath + file;

	auto p = std::filesystem::path(fileStr);
	if (!std::filesystem::exists(p))
	{
		LOG(Error, "File NOT Exist!");
		assert(false);
	}

	_scene = _importer->ReadFile(
		Utils::ToString(fileStr),
		aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_GenUVCoords |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	if (_scene == nullptr)
	{
		LOG(Error, "Assimp Importer Read File Failed!");
		assert(false);
	}
}

void Converter::ExportModelData(wstring savePath)
{
	wstring finalPath = _modelSaveRoot + savePath + L".mesh";
	
	ReadModelDataFromAssimp(_scene->mRootNode, -1, -1);
	WriteCustomMeshFile(finalPath);
}

void Converter::ReadModelDataFromAssimp(aiNode* node, int32 index, int32 parent)
{
	shared_ptr<ASBone> bone = make_shared<ASBone>();
	bone->index = index;
	bone->parent = parent;
	bone->name = node->mName.C_Str();

	// Relative Transform
	Matrix transform(node->mTransformation[0]);
	bone->transform = transform.Transpose();    // Assimp : using Col-Major

	// Root (Local)
	Matrix matParent = Matrix::Identity;
	if (parent >= 0)
		matParent = _bones[parent]->transform;

	// Transform To Parent
	bone->transform = bone->transform * matParent;

	_bones.push_back(bone);

	// Mesh
	ReadMeshDataFromAssimp(node, index);

	for (uint32 i = 0; i < node->mNumChildren; i++)
		ReadModelDataFromAssimp(node->mChildren[i], _bones.size(), index);
}

void Converter::ReadMeshDataFromAssimp(aiNode* node, int32 bone)
{
	if (node->mNumMeshes < 1)
		return;

	shared_ptr<ASMesh> mesh = make_shared<ASMesh>();
	mesh->name = node->mName.C_Str();
	mesh->boneIndex = bone;

	for (uint32 i = 0; i < node->mNumMeshes; i++)
	{
		uint32 index = node->mMeshes[i];
		const aiMesh* srcMesh = _scene->mMeshes[index];

		// Material Name
		const aiMaterial* material = _scene->mMaterials[srcMesh->mMaterialIndex];
		mesh->materialName = material->GetName().C_Str();

		const uint32 startVertex = mesh->vertices.size();

		for (uint32 v = 0; v < srcMesh->mNumVertices; v++)
		{
			// Vertex
			StaticMeshVertexType vertex;
			::memcpy(&vertex.position, &srcMesh->mVertices[v], sizeof(Vec3));

			// UV
			if (srcMesh->HasTextureCoords(0))
				::memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Vec2));

			// Normal
			if (srcMesh->HasNormals())
				::memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Vec3));

			mesh->vertices.push_back(vertex);
		}

		// Index
		for (uint32 f = 0; f < srcMesh->mNumFaces; f++)
		{
			aiFace& face = srcMesh->mFaces[f];

			for (uint32 k = 0; k < face.mNumIndices; k++)
				mesh->indices.push_back(face.mIndices[k] + startVertex);
		}
	}

	_meshes.push_back(mesh);
}

void Converter::WriteCustomMeshFile(wstring finalPath)
{
	auto path = filesystem::path(finalPath);

	// 폴더가 없으면 만든다.
	filesystem::create_directory(path.parent_path());

	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(finalPath, FileMode::Write);

	// Bone Data
	file->Write<uint32>(_bones.size());
	for (shared_ptr<ASBone>& bone : _bones)
	{
		file->Write<int32>(bone->index);
		file->Write<string>(bone->name);
		file->Write<int32>(bone->parent);
		file->Write<Matrix>(bone->transform);
	}

	// Mesh Data
	file->Write<uint32>(_meshes.size());
	for (shared_ptr<ASMesh>& meshData : _meshes)
	{
		file->Write<string>(meshData->name);
		file->Write<int32>(meshData->boneIndex);
		file->Write<string>(meshData->materialName);

		// Vertex Data
		file->Write<uint32>(meshData->vertices.size());
		file->Write(&meshData->vertices[0], sizeof(StaticMeshVertexType) * meshData->vertices.size());

		// Index Data
		file->Write<uint32>(meshData->indices.size());
		file->Write(&meshData->indices[0], sizeof(uint32) * meshData->indices.size());
	}
}

void Converter::ExportMaterialData(wstring savePath)
{
	wstring finalPath = _materialSaveRoot + savePath + L".xml";

	std::filesystem::path Path = savePath;
	wstring parentFolderName = Path.parent_path().wstring();
	wstring rawFbxFilePath = _rawAssetPath + parentFolderName;

	ReadMaterialDataFromAssimp(rawFbxFilePath);
	WriteCustomMaterialFile(finalPath);
}

void Converter::ReadMaterialDataFromAssimp(const wstring& pathForMissing)
{
	for (uint32 i = 0; i < _scene->mNumMaterials; i++)
	{
		aiMaterial* srcMaterial = _scene->mMaterials[i];

		shared_ptr<ASMaterial> material = make_shared<ASMaterial>();
		material->name = srcMaterial->GetName().C_Str();

		aiColor3D color;
		// Ambient
		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->ambient = Color(color.r, color.g, color.b, 1.f);

		// Diffuse
		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->diffuse = Color(color.r, color.g, color.b, 1.f);

		// Specular
		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->specular = Color(color.r, color.g, color.b, 1.f);
		srcMaterial->Get(AI_MATKEY_SHININESS, material->specular.w);

		// Emissive
		srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		material->emissive = Color(color.r, color.g, color.b, 1.0f);

		aiString file;

		// Diffuse Texture
		aiReturn returnVal = srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		SetFoundTexturePath(material->diffuseFile, file.C_Str(), pathForMissing, MissingTextureFindKeyword::Diffuse);

		// Specular Texture
		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		SetFoundTexturePath(material->specularFile, file.C_Str(), pathForMissing, MissingTextureFindKeyword::Specular);

		// Normal Texture
		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		SetFoundTexturePath(material->normalFile, file.C_Str(), pathForMissing, MissingTextureFindKeyword::Normal);

		_materials.push_back(material);
	}
}

void Converter::WriteCustomMaterialFile(wstring finalPath)
{
	auto path = filesystem::path(finalPath);

	// 폴더가 없으면 만든다.
	filesystem::create_directory(path.parent_path());

	string folder = path.parent_path().string();

	shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();

	tinyxml2::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	tinyxml2::XMLElement* root = document->NewElement("Materials");
	document->LinkEndChild(root);

	for (shared_ptr<ASMaterial> material : _materials)
	{
		tinyxml2::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		tinyxml2::XMLElement* element = nullptr;

		element = document->NewElement("Name");
		element->SetText(material->name.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("DiffuseFile");
		element->SetText(WriteTexture(folder, material->diffuseFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("SpecularFile");
		element->SetText(WriteTexture(folder, material->specularFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("NormalFile");
		element->SetText(WriteTexture(folder, material->normalFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("Ambient");
		element->SetAttribute("R", material->ambient.x);
		element->SetAttribute("G", material->ambient.y);
		element->SetAttribute("B", material->ambient.z);
		element->SetAttribute("A", material->ambient.w);
		node->LinkEndChild(element);

		element = document->NewElement("Diffuse");
		element->SetAttribute("R", material->diffuse.x);
		element->SetAttribute("G", material->diffuse.y);
		element->SetAttribute("B", material->diffuse.z);
		element->SetAttribute("A", material->diffuse.w);
		node->LinkEndChild(element);

		element = document->NewElement("Specular");
		element->SetAttribute("R", material->specular.x);
		element->SetAttribute("G", material->specular.y);
		element->SetAttribute("B", material->specular.z);
		element->SetAttribute("A", material->specular.w);
		node->LinkEndChild(element);

		element = document->NewElement("Emissive");
		element->SetAttribute("R", material->emissive.x);
		element->SetAttribute("G", material->emissive.y);
		element->SetAttribute("B", material->emissive.z);
		element->SetAttribute("A", material->emissive.w);
		node->LinkEndChild(element);
	}

	document->SaveFile(Utils::ToString(finalPath).c_str());

	_materials.clear();
}

string Converter::WriteTexture(string saveFolder, string file)
{
	/*if (file.empty())
		return string();*/

	string fileName = filesystem::path(file).filename().string();
	string folderName = filesystem::path(saveFolder).filename().string();

	const aiTexture* srcTexture = _scene->GetEmbeddedTexture(file.c_str());
	if (srcTexture)
	{
		string pathStr = saveFolder + fileName;

		if (srcTexture->mHeight == 0)
		{
			// 압축된 데이터 : 저장
			shared_ptr<FileUtils> file = make_shared<FileUtils>();
			file->Open(Utils::ToWString(pathStr), FileMode::Write);
			file->Write(srcTexture->pcData, srcTexture->mWidth);
		}
		else
		{
			// 비압축 데이터인 경우 바로 Directx 셋팅
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
			desc.Width = srcTexture->mWidth;
			desc.Height = srcTexture->mHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_IMMUTABLE;

			D3D11_SUBRESOURCE_DATA subResource = { 0 };
			subResource.pSysMem = srcTexture->pcData;

			ComPtr<ID3D11Texture2D> texture;
			HRESULT hr = DEVICE->CreateTexture2D(&desc, &subResource, texture.GetAddressOf());
			check(hr);

			DirectX::ScratchImage img;
			::CaptureTexture(DEVICE.Get(), CONTEXT.Get(), texture.Get(), img);

			// Save To File
			hr = DirectX::SaveToDDSFile(*img.GetImages(), DirectX::DDS_FLAGS_NONE, Utils::ToWString(fileName).c_str());
			check(hr);
		}
	}
	else
	{
		string originStr = (filesystem::path(_rawAssetPath) / folderName / file).string();
		//string originStr = file;
		Utils::Replace(originStr, "\\", "/");

		string pathStr = (filesystem::path(saveFolder) / fileName).string();
		Utils::Replace(pathStr, "\\", "/");

		if (!::CopyFileA(originStr.c_str(), pathStr.c_str(), false))
		{
			DWORD error = GetLastError();
			string msg = "CoppyFile Failed error No: " + error;
			LOG(Warning, msg);
		}
	}

	return fileName;
}

void Converter::SetFoundTexturePath(string& setTarget, const string& path, const wstring& pathForMissing, MissingTextureFindKeyword keyword)
{
	if (!path.empty() && path[0] != '*')
	{
		setTarget = path;
	}
	else
	{
		string missingPath = FindMissingTextureInFBXFolder(pathForMissing, keyword);
		if (missingPath.empty())
		{
			LOG(Log, "Texture NOT Found");
		}
		setTarget = missingPath;
	}
}

string Converter::FindMissingTextureInFBXFolder(const wstring& findTargetPath, MissingTextureFindKeyword keyword)
{
	namespace fs = std::filesystem;

	fs::path targetPath(findTargetPath);

	if (!fs::exists(targetPath) || !fs::is_directory(targetPath))
	{
		string msg = "Wrong Path: " + Utils::ToString(findTargetPath);
		LOG(Error, msg);
		return "";
	}

	for (const auto& entry : fs::recursive_directory_iterator(targetPath))
	{
		if (!entry.is_regular_file())
			continue;

		fs::path filepath = entry.path();
		std::wstring extention = filepath.extension().wstring();

		// 지원하는 이미지 확장자
		if (extention != L".png" && extention != L".jpg" && extention != L".dds" && extention != L".tga")
			continue;

		// 키워드에 해당하는 파일명 포함 여부 확인
		std::wstring filename = filepath.filename().wstring();
		std::wstring lowerFilename = filename;
		std::transform(lowerFilename.begin(), lowerFilename.end(), lowerFilename.begin(), ::towlower);

		bool match = false;
		switch (keyword)
		{
		case MissingTextureFindKeyword::Diffuse:
			match = lowerFilename.find(L"diffuse") != std::wstring::npos || lowerFilename.find(L"albedo") != std::wstring::npos;
			break;
		case MissingTextureFindKeyword::Normal:
			match = lowerFilename.find(L"normal") != std::wstring::npos || lowerFilename.find(L"nrm") != std::wstring::npos;
			break;
		case MissingTextureFindKeyword::Specular:
			match = lowerFilename.find(L"specular") != std::wstring::npos || lowerFilename.find(L"spec") != std::wstring::npos;
			break;
		default:
			break;
		}

		if (match)
		{
			LOG(Log, "Found Texture!");

			fs::path folder = filepath.parent_path().filename();  // "c"
			fs::path file = filepath.filename();  
			fs::path cutPath = folder / file;

			string foundStr = cutPath.string();
			Utils::Replace(foundStr, "\\", "/");

			return foundStr;
		}
	}

	return "";
}

void Converter::ReadObjFile(const wstring& path)
{
	wstring fileStr = _rawAssetPath + path;

	auto p = std::filesystem::path(fileStr);
	if (!std::filesystem::exists(p))
	{
		LOG(Error, "File NOT Exist!");
		assert(false);
	}

	const aiScene* scene = _importer->ReadFile(
		Utils::ToString(fileStr),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "Assimp error: " << _importer->GetErrorString() << std::endl;
		return;
	}

	std::cout << "Loaded: " << Utils::ToString(fileStr) << std::endl;
	PrintMaterialInfo(scene);
}

void Converter::PrintMaterialInfo(const aiScene* scene)
{
	for (uint32_t i = 0; i < scene->mNumMaterials; ++i)
	{
		const aiMaterial* material = scene->mMaterials[i];
		aiString name;
		material->Get(AI_MATKEY_NAME, name);

		std::cout << "Material [" << i << "]: " << name.C_Str() << std::endl;

		// Diffuse Color
		aiColor3D color(0.f, 0.f, 0.f);
		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
		{
			std::cout << "  Diffuse Color: (" << color.r << ", " << color.g << ", " << color.b << ")\n";
		}

		// Diffuse Texture
		aiString texturePath;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
		{
			std::cout << "  Diffuse Texture: " << texturePath.C_Str() << std::endl;
		}
		else
		{
			std::cout << "  Diffuse Texture: (none)\n";
		}

		std::cout << std::endl;
	}
}

void Converter::Cleanup()
{
	_scene = nullptr;

	_bones.clear();
	_meshes.clear();
	_materials.clear();
}