#pragma once

struct ShaderInfo
{
	enum class ShaderVersion
	{
		Ver5_0,
		Ver5_1,
		End
	};

public:

	ShaderInfo(const wstring& shaderFileName, string vsEntryName = "VS", string psEntryName = "PS", ShaderVersion version = ShaderVersion::Ver5_0);
	~ShaderInfo();

	bool IsValidVersion(ShaderVersion version)
	{
		return version == ShaderVersion::Ver5_0 || version == ShaderVersion::Ver5_1;
	}

	wstring _shaderPath{};
	string _vsEntryName{}; 
	string _psEntryName{};
	string _vsVersion{};
	string _psVersion{};
};

