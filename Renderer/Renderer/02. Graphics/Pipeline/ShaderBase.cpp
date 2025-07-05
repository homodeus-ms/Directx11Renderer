#include "pch.h"
#include "ShaderBase.h"

ShaderBase::ShaderBase(ComPtr<ID3D11Device> device)
	: _device(device)
{
}

ShaderBase::~ShaderBase()
{
}

void ShaderBase::CompileShaderAndSetBlob(const wstring& shaderPath, const string& funcName, const string& version)
{
	_shaderPath = shaderPath;
	_funcName = funcName;

	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = ::D3DCompileFromFile(
		shaderPath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		funcName.c_str(),
		version.c_str(),
		compileFlag,
		0,
		_blob.GetAddressOf(),
		nullptr);

	check(hr);
}
