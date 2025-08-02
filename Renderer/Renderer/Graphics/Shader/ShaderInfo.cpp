#include "pch.h"
#include "ShaderInfo.h"

ShaderInfo::ShaderInfo(const wstring& shaderName, string vsEntryName, string psEntryName, EShaderVersion version)
	: ShaderInfo(shaderName, shaderName, vsEntryName, psEntryName, version)
	//: _shaderPath(L"..\\Client\\Shaders\\" + shaderName)
	//, _vsShaderPath(_shaderPath)
	//, _psShaderPath(_shaderPath)
	//, _vsEntryName(vsEntryName)
	//, _psEntryName(psEntryName)
{
	/*if (!IsValidVersion(version))
	{
		LOG(Error, "Not Supported Shader Version(Use 5.0 or 5.1)");
		assert(false);
	}
	
	switch (version)
	{
	case EShaderVersion::Ver5_0:
	{
		_vsVersion = "vs_5_0";
		_psVersion = "ps_5_0";
		break;
	}
	case EShaderVersion::Ver5_1:
	{
		_vsVersion = "vs_5_1";
		_psVersion = "ps_5_1";
		break;
	}
	default:
		assert(false);
	}*/
}

ShaderInfo::ShaderInfo(const wstring& vsShaderName, const wstring& psShaderName, string vsEntryName, string psEntryName, EShaderVersion version)
	: _shaderPath(L"..\\Client\\Shaders\\" + vsShaderName)
	, _vsShaderPath(L"..\\Client\\Shaders\\" + vsShaderName)
	, _psShaderPath(L"..\\Client\\Shaders\\" + psShaderName)
	, _vsEntryName(vsEntryName)
	, _psEntryName(psEntryName)
{
	if (!IsValidVersion(version))
	{
		LOG(Error, "Not Supported Shader Version(Use 5.0 or 5.1)");
		assert(false);
	}

	switch (version)
	{
	case EShaderVersion::Ver5_0:
	{
		_vsVersion = "vs_5_0";
		_psVersion = "ps_5_0";
		break;
	}
	case EShaderVersion::Ver5_1:
	{
		_vsVersion = "vs_5_1";
		_psVersion = "ps_5_1";
		break;
	}
	default:
		assert(false);
	}
}

ShaderInfo::~ShaderInfo()
{
}

void ShaderInfo::AddGSShaderInfo(string gsEntryName, EShaderVersion version)
{
	_gsEntryName = gsEntryName;
	_gsVersion = version == EShaderVersion::Ver5_0 ? "gs_5_0" : "gs_5_1";
}
