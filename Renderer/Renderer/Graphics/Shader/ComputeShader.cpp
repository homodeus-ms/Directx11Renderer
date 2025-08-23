#include "pch.h"
#include "ComputeShader.h"

ComputeShader::ComputeShader()
{
}

ComputeShader::~ComputeShader()
{
}

void ComputeShader::Create(const wstring& shaderPath, const string& entryName, const string& version)
{
	wstring path = L"..\\Client\\Shaders\\CS\\" + shaderPath;
	LoadShaderFromFile(path, entryName, version);

	HRESULT hr = DEVICE->CreateComputeShader(
		_blob->GetBufferPointer(),
		_blob->GetBufferSize(),
		nullptr,
		_computeShader.GetAddressOf());

	check(hr);
}
