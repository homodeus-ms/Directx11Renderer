#include "pch.h"
#include "Texture.h"
#include <filesystem>

Texture::Texture()
	: Super(EResourceType::Texture)
{
}

Texture::~Texture()
{
}

void Texture::Load(const wstring& path)
{
	std::filesystem::path filepath(path);
	wstring ext = filepath.extension().wstring();

	DirectX::TexMetadata md;

	if (ext == L".tga")
	{
		HRESULT hr = DirectX::LoadFromTGAFile(path.c_str(), &md, _img);
		check(hr);
	}
	else
	{
		HRESULT hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, _img);
		check(hr);
	}

	HRESULT hr = ::CreateShaderResourceView(DEVICE.Get(), _img.GetImages(), _img.GetImageCount(), md, _shaderResourceView.GetAddressOf());
	check(hr);

	_size.x = static_cast<float>(md.width);
	_size.y = static_cast<float>(md.height);
}

