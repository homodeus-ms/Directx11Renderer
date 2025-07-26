#include "pch.h"
#include "ShadowCubeTexture.h"
#include "Structs/ShaderParameterTypes.h"
#include "Graphics/RenderPass/ShadowMapResources.h"

ShadowCubeTexture::ShadowCubeTexture()
{
}

ShadowCubeTexture::~ShadowCubeTexture()
{
}

void ShadowCubeTexture::CreateTexture()
{
	// Texture
	{
		D3D11_TEXTURE2D_DESC desc;
		::ZeroMemory(&desc, sizeof(desc));

		desc.Width = SHADOW_MAP_SIZE;
		desc.Height = SHADOW_MAP_SIZE;
		desc.MipLevels = 1;
		desc.ArraySize = 6;
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		HRESULT hr = DEVICE->CreateTexture2D(&desc, nullptr, _texture.GetAddressOf());
		check(hr);
	}

	// SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MipLevels = 1;
		srvDesc.TextureCube.MostDetailedMip = 0;

		HRESULT hr = DEVICE->CreateShaderResourceView(_texture.Get(), &srvDesc, _SRV.GetAddressOf());
		check(hr);

		// SRV Binding Info
		_srvBindingInfo = make_shared<SRVBindingInfo>();
		_srvBindingInfo->slot = SHADOW_SLOT_NUM + MAX_SHADOW_MAP_COUNT;
		_srvBindingInfo->stage = EShaderStage::PsStage;
		_srvBindingInfo->srv = _SRV;
	}

	// RTV
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		rtvDesc.Texture2DArray.MipSlice = 0;
		rtvDesc.Texture2DArray.FirstArraySlice = 0;
		rtvDesc.Texture2DArray.ArraySize = 6;

		HRESULT hr = DEVICE->CreateRenderTargetView(_texture.Get(), &rtvDesc, _RTV.GetAddressOf());
		check(hr);
	}
}
