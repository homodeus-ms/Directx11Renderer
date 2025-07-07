#include "pch.h"
#include "Shader.h"

Shader::Shader()
{

}

Shader::~Shader()
{
}

void Shader::LoadShaderFromFile(const wstring& shaderPath, const string& entryName, const string& version)
{
	ComPtr<ID3DBlob> error;

	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	HRESULT hr = ::D3DCompileFromFile(
		shaderPath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryName.c_str(),
		version.c_str(),
		compileFlag,
		0,
		_blob.GetAddressOf(),
		error.GetAddressOf());

	if (FAILED(hr))
	{
		if (error != NULL)
		{
			string str = (const char*)error->GetBufferPointer();
			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
		}
		assert(false);
	}

}
