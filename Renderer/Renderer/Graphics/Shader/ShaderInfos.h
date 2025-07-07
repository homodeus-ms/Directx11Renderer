#pragma once

struct ShaderInfos
{
	enum class ShaderVersion
	{
		Ver5_0,
		Ver5_1,
		End
	};

public:
	ShaderInfos(const wstring& shaderFileName, string vsEntryName = "VS", string psEntryName = "PS", ShaderVersion version = ShaderVersion::Ver5_0);
	~ShaderInfos();

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

