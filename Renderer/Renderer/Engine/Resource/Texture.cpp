#include "pch.h"
#include "Texture.h"

Texture::Texture()
	: Super(ResourceType::Texture)
{
}

Texture::~Texture()
{
}

void Texture::Load(const wstring& path)
{
	DirectX::TexMetadata md;
	HRESULT hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, _img);
	check(hr);

	hr = ::CreateShaderResourceView(DEVICE.Get(), _img.GetImages(), _img.GetImageCount(), md, _shaderResourveView.GetAddressOf());
	check(hr);

	_size.x = md.width;
	_size.y = md.height;
}
