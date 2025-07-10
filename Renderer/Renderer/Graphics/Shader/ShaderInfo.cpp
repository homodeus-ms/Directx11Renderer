#include "pch.h"
#include "ShaderInfo.h"

ShaderInfo::ShaderInfo(const wstring& shaderFileName, string vsEntryName, string psEntryName, ShaderVersion version)
	: _shaderPath(L"..\\Client\\Shaders\\" + shaderFileName)
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
	case ShaderVersion::Ver5_0:
	{
		_vsVersion = "vs_5_0";
		_psVersion = "ps_5_0";
		break;
	}
	case ShaderVersion::Ver5_1:
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
