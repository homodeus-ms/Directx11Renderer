#include "pch.h"
#include "PixelShader.h"

PixelShader::PixelShader()
{
}

PixelShader::~PixelShader()
{
}

void PixelShader::Create(const wstring& shaderPath, const string& entryName, const string& version)
{
	LoadShaderFromFile(shaderPath, entryName, version);

	HRESULT hr = DEVICE->CreatePixelShader(
		_blob->GetBufferPointer(),
		_blob->GetBufferSize(),
		nullptr,
		_pixelShader.GetAddressOf());

	check(hr);
}
