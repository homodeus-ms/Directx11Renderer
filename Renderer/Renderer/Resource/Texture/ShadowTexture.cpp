#include "pch.h"
#include "ShadowTexture.h"
#include "Graphics/RenderPass/ShadowMapResources.h"
#include "Structs/ShaderParameterTypes.h"

ShadowTexture::ShadowTexture()
{
}

ShadowTexture::~ShadowTexture()
{
}

void ShadowTexture::CreateTexture()
{
	// Texture
	{
		D3D11_TEXTURE2D_DESC desc;
		::ZeroMemory(&desc, sizeof(desc));
		desc.Width = SHADOW_MAP_SIZE;
		desc.Height = SHADOW_MAP_SIZE;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32_TYPELESS; // 깊이와 SRV 둘 다 사용하려면 TYPELESS
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		HRESULT hr = DEVICE->CreateTexture2D(&desc, nullptr, _texture.GetAddressOf());
		check(hr);
	}

	// DSV
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Format = DXGI_FORMAT_D32_FLOAT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		HRESULT hr = DEVICE->CreateDepthStencilView(_texture.Get(), &desc, _DSV.GetAddressOf());
		check(hr);
	}

	// SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		::ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_R32_FLOAT;    // Shader에서 사용할 format
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;

		HRESULT hr = DEVICE->CreateShaderResourceView(_texture.Get(), &desc, _SRV.GetAddressOf());
		check(hr);

		// SRV Binding Info
		_srvBindingInfo = make_shared<SRVBindingInfo>();
		_srvBindingInfo->slot = static_cast<uint8>(EShadowTextureType::Shadow);
		_srvBindingInfo->stage = EShaderStage::PsStage;
		_srvBindingInfo->srv = _SRV;
	}
}
