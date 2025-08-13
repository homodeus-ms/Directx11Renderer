#include "pch.h"
#include "Texture3D.h"
#include "Structs/ShaderParameterTypes.h"

Texture3D::Texture3D()
{
}

Texture3D::~Texture3D()
{
}

void Texture3D::CreateTexture(uint32 size, const vector<Vec3>& data)
{
	D3D11_TEXTURE3D_DESC desc = {};
	desc.Width = size;
	desc.Height = size;
	desc.Depth = size;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = data.data();
	initData.SysMemPitch = sizeof(float) * 3 * size;
	initData.SysMemSlicePitch = initData.SysMemPitch * size;

	DEVICE->CreateTexture3D(&desc, &initData, &_texture);

	// SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
	srvDesc.Texture3D.MipLevels = 1;
	
	DEVICE->CreateShaderResourceView(_texture.Get(), &srvDesc, _SRV.GetAddressOf());

	_srvBindingInfo = make_shared<SRVBindingInfo>();
	_srvBindingInfo->slot = static_cast<uint8>(EFilterTextureType::LUT);
	_srvBindingInfo->stage = EShaderStage::PsStage;
	_srvBindingInfo->srv = _SRV;
}
