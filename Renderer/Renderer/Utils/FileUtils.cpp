
#include "pch.h"
#include "FileUtils.h"
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

FileUtils::FileUtils()
{

}

FileUtils::~FileUtils()
{
	if (_handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(_handle);
		_handle = INVALID_HANDLE_VALUE;
	}
}


void FileUtils::Open(wstring filePath, FileMode mode)
{
	if (mode == FileMode::Write)
	{
		_handle = ::CreateFile(
			filePath.c_str(),
			GENERIC_WRITE,
			0,
			nullptr,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
	}
	else
	{
		_handle = ::CreateFile
		(
			filePath.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
	}

	assert(_handle != INVALID_HANDLE_VALUE);
}


void FileUtils::Write(void* data, uint32 dataSize)
{
	uint32 numOfBytes = 0;
	assert(::WriteFile(_handle, data, dataSize, reinterpret_cast<LPDWORD>(&numOfBytes), nullptr));
}

void FileUtils::Write(const string& data)
{
	uint32 size = (uint32)data.size();
	Write(size);

	if (data.size() == 0)
		return;

	Write((void*)data.data(), size);
}

void FileUtils::Read(void** data, uint32 dataSize)
{
	uint32 numOfBytes = 0;
	assert(::ReadFile(_handle, *data, dataSize, reinterpret_cast<LPDWORD>(&numOfBytes), nullptr));
}

void FileUtils::Read(OUT string& data)
{
	uint32 size = Read<uint32>();

	if (size == 0)
		return;

	char* temp = new char[size + 1];
	temp[size] = 0;
	Read((void**)&temp, size);
	data = temp;
	delete[] temp;
}

void FileUtils::CombineAndSaveMetallicRoughness(const string& savePath, const string& metallicName, const string& roughnessName)
{
	int mw, mh, mc;
	int rw, rh, rc;

	// Metallic texture 로드
	const string metalFullPath = savePath + "\\" + metallicName;
	unsigned char* metallicData = stbi_load(metalFullPath.c_str(), &mw, &mh, &mc, 1);
	if (!metallicData)
	{
		LOG(Warning, "Failed to load metallic texture");
		return;
	}

	// Roughness texture 로드
	const string roughnessFullPath = savePath + "\\" + roughnessName;
	unsigned char* roughnessData = stbi_load(roughnessFullPath.c_str(), &rw, &rh, &rc, 1);
	if (!roughnessData)
	{
		LOG(Warning, "Failed to load roughness texture");
		stbi_image_free(metallicData);
		return;
	}

	// 해상도 체크
	if (mw != rw || mh != rh)
	{
		LOG(Warning, "Error: texture size mismatch");
		stbi_image_free(metallicData);
		stbi_image_free(roughnessData);
		return;
	}

	// 결과 텍스처 데이터 (RGBA)
	vector<unsigned char> combined(mw * mh * 4);

	for (int i = 0; i < mw * mh; i++)
	{
		unsigned char m = metallicData[i];   // metallic value
		unsigned char r = roughnessData[i];  // roughness value

		combined[i * 4 + 0] = 0; // R 채널 = 0
		combined[i * 4 + 1] = r; // G 채널 = roughness
		combined[i * 4 + 2] = m; // B 채널 = metallic
		combined[i * 4 + 3] = 255; // A 채널 = 255 (불투명)
	}

	
	string saveName = metalFullPath;
	size_t dotPos = saveName.find_last_of('.');
	if (dotPos != std::string::npos) {
		saveName.insert(dotPos, "Roughness");
	}

	// PNG로 저장
	if (!stbi_write_png(saveName.c_str(), mw, mh, 4, combined.data(), mw * 4))
	{
		LOG(Warning, "Failed to save texture");
	}

	stbi_image_free(metallicData);
	stbi_image_free(roughnessData);
}
