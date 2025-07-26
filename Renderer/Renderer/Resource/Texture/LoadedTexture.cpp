#include "pch.h"
#include "Resource/Texture/LoadedTexture.h"
#include <filesystem>
#include <directxtk/DDSTextureLoader.h>


LoadedTexture::LoadedTexture()
{
}

LoadedTexture::~LoadedTexture()
{
}

void LoadedTexture::Load(const wstring& path)
{
	std::filesystem::path filepath(path);
	wstring ext = filepath.extension().wstring();

	DirectX::TexMetadata md;

	if (ext == L".tga")
	{
		HRESULT hr = DirectX::LoadFromTGAFile(path.c_str(), &md, _img);
		check(hr);
	}
	else if (ext == L".dds")
	{
		ComPtr<ID3D11Resource> cubeTexture;

		HRESULT hr = ::CreateDDSTextureFromFileEx(
			DEVICE.Get(),                            // ID3D11Device*
			path.c_str(),  // DDS ť��� ���
			0,                                  // maxsize (0�̸� ���� ũ��)
			D3D11_USAGE_DEFAULT,                // usage
			D3D11_BIND_SHADER_RESOURCE,         // bindFlags
			0,                                  // cpuAccessFlags
			D3D11_RESOURCE_MISC_TEXTURECUBE,    // miscFlags �� ť��� �ٽ�!
			DDS_LOADER_FLAGS(false),                 // loadFlags
			cubeTexture.GetAddressOf(),         // �ؽ�ó ���
			_SRV.GetAddressOf(),             // SRV ���
			nullptr                             // alphaMode (�ɼ�)
		);

		return;
	}
	else
	{
		HRESULT hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, _img);
		check(hr);
	}

	HRESULT hr = ::CreateShaderResourceView(DEVICE.Get(), _img.GetImages(), _img.GetImageCount(), md, _SRV.GetAddressOf());
	check(hr);

	_size.x = static_cast<float>(md.width);
	_size.y = static_cast<float>(md.height);
}

