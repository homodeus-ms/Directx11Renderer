#include "pch.h"
#include "GeometryShader.h"

GeometryShader::GeometryShader()
{
}

GeometryShader::~GeometryShader()
{
}

void GeometryShader::Create(const wstring& shaderPath, const string& entryName, const string& version)
{
	LoadShaderFromFile(shaderPath, entryName, version);

	HRESULT hr = DEVICE->CreateGeometryShader(
		_blob->GetBufferPointer(),
		_blob->GetBufferSize(),
		nullptr,
		_geometryShader.GetAddressOf());

	check(hr);
}
