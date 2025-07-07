#include "pch.h"
#include "VertexShader.h"

VertexShader::VertexShader()
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::Create(const wstring& shaderPath, const string& entryName, const string& version)
{
	LoadShaderFromFile(shaderPath, entryName, version);

	HRESULT hr = DEVICE->CreateVertexShader(
		_blob->GetBufferPointer(), 
		_blob->GetBufferSize(), 
		nullptr, 
		_vertexShader.GetAddressOf());

	check(hr);
}
