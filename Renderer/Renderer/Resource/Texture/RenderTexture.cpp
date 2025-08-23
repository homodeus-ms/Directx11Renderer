#include "pch.h"
#include "RenderTexture.h"
#include "Structs/ShaderParameterTypes.h"

RenderTexture::RenderTexture()
{
}

RenderTexture::~RenderTexture()
{
}

void RenderTexture::CreateTexture(uint32 w, uint32 h, uint8 srvSlotNum)
{
    {
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = w;
        desc.Height = h;
        desc.MipLevels = desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; //  이미지 처리용도
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE |
            D3D11_BIND_RENDER_TARGET |
            D3D11_BIND_UNORDERED_ACCESS;
        desc.MiscFlags = 0;
        desc.CPUAccessFlags = 0;

        HRESULT hr = DEVICE->CreateTexture2D(&desc, NULL, _texture.GetAddressOf());
        check(hr);
    }

    {
        D3D11_RENDER_TARGET_VIEW_DESC desc;
        desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipSlice = 0;

        HRESULT hr = DEVICE->CreateRenderTargetView(_texture.Get(), &desc, _RTV.GetAddressOf());
        check(hr);
    }

    {
        HRESULT hr = DEVICE->CreateShaderResourceView(_texture.Get(), nullptr, _SRV.GetAddressOf());
        check(hr);

        _srvBindingInfo = make_shared<SRVBindingInfo>();
        _srvBindingInfo->slot = srvSlotNum;
        _srvBindingInfo->stage = EShaderStage::PsStage;
        _srvBindingInfo->srv = _SRV;
    }
}
