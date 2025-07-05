#include "pch.h"
#include "VertexShader.h"

VertexShader::VertexShader(ComPtr<ID3D11Device> device)
	: Super(device)
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::Create(const wstring& shaderPath, const string& funcName, const string& version)
{
	CompileShaderAndSetBlob(L"Triangle.hlsl", "VS", "vs_5_0");

	HRESULT hr = _device->CreateVertexShader(
		_blob->GetBufferPointer(),
		_blob->GetBufferSize(),
		nullptr,
		_vertexShader.GetAddressOf());

	check(hr);
}
