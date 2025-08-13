#include "pch.h"
#include "Resource/Texture/LoadedTexture.h"
#include <filesystem>
#include <directxtk/DDSTextureLoader.h>
#include <dxgi.h>                       
#include <dxgi1_4.h>                    
#include <fp16.h>


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

		if (md.format == DXGI_FORMAT_R8G8B8A8_UNORM)
		{
			DirectX::ScratchImage converted;
			hr = DirectX::Convert(_img.GetImages(), _img.GetImageCount(), md, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, TEX_FILTER_DEFAULT, 0.0f, converted);
			check(hr);

			_img = std::move(converted);
			md.format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		}
	}
	else if (ext == L".dds")
	{
		ComPtr<ID3D11Resource> cubeTexture;

		HRESULT hr = ::CreateDDSTextureFromFileEx(
			DEVICE.Get(),                            // ID3D11Device*
			path.c_str(),  // DDS 큐브맵 경로
			0,                                  // maxsize (0이면 원본 크기)
			D3D11_USAGE_DEFAULT,                // usage
			D3D11_BIND_SHADER_RESOURCE,         // bindFlags
			0,                                  // cpuAccessFlags
			D3D11_RESOURCE_MISC_TEXTURECUBE,    // miscFlags ← 큐브맵 핵심!
			DDS_LOADER_FLAGS(false),                 // loadFlags
			cubeTexture.GetAddressOf(),         // 텍스처 출력
			_SRV.GetAddressOf(),             // SRV 출력
			nullptr                             // alphaMode (옵션)
		);

		return;
	}
	else if (ext == L".exr")
	{
		HRESULT hr = GetMetadataFromEXRFile(path.c_str(), md);
		check(hr);
		
		hr = LoadFromEXRFile(path.c_str(), NULL, _img);
	}
	else
	{
		HRESULT hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_FORCE_SRGB, &md, _img);
		check(hr);
	}

	_size.x = static_cast<float>(md.width);
	_size.y = static_cast<float>(md.height);

	// MipMap 생성
	const Image* loadedImage = _img.GetImage(0, 0, 0);
	if (!loadedImage)
		return;

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = static_cast<UINT>(loadedImage->width);
	desc.Height = static_cast<UINT>(loadedImage->height);
	desc.MipLevels = 0;     // 전체 mipmap 생성
	desc.ArraySize = 1;
	desc.Format = loadedImage->format;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	HRESULT hr = DEVICE->CreateTexture2D(&desc, nullptr, _texture.GetAddressOf());
	check(hr);

	CONTEXT->UpdateSubresource(_texture.Get(), 0, nullptr, loadedImage->pixels,
		static_cast<UINT>(loadedImage->rowPitch),
		static_cast<UINT>(loadedImage->slicePitch)
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = -1;     // 전체 mip 사용
	srvDesc.Texture2D.MostDetailedMip = 0;
	
	hr = DEVICE->CreateShaderResourceView(_texture.Get(), &srvDesc, _SRV.GetAddressOf());
	check(hr);
	
	CONTEXT->GenerateMips(_SRV.Get());

	// TEMP : For Check
	D3D11_TEXTURE2D_DESC tempDesc = {};
	_texture->GetDesc(&tempDesc);
}

