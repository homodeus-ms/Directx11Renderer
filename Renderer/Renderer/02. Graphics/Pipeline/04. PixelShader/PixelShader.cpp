#include "pch.h"
#include "PixelShader.h"

PixelShader::PixelShader(ComPtr<ID3D11Device> device)
	: Super(device)
{
}

PixelShader::~PixelShader()
{
}

void PixelShader::Create(const wstring& shaderPath, const string& funcName, const string& version)
{
	CompileShaderAndSetBlob(L"Triangle.hlsl", "PS", "vs_5_0");

	HRESULT hr = _device->CreatePixelShader(
		_blob->GetBufferPointer(),
		_blob->GetBufferSize(),
		nullptr,
		_pixelShader.GetAddressOf());

	check(hr);
}
