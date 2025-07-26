#pragma once

struct ShaderInfo
{
	enum class EShaderVersion
	{
		Ver5_0,
		Ver5_1,
		End
	};

public:

	ShaderInfo(const wstring& shaderFileName, string vsEntryName = "VS", string psEntryName = "PS", EShaderVersion version = EShaderVersion::Ver5_0);
	~ShaderInfo();

	// TEMP : VS�� �ִ� ���̴� ���Ͽ� ���� ����Ѵٰ� ����
	void AddGSShaderInfo(string gsEntryName = "GS", EShaderVersion version = EShaderVersion::Ver5_0);

	bool IsValidVersion(EShaderVersion version)
	{
		return version == EShaderVersion::Ver5_0 || version == EShaderVersion::Ver5_1;
	}

	wstring _shaderPath{};
	string _vsEntryName{}; 
	string _psEntryName{};
	string _vsVersion{};
	string _psVersion{};
	string _gsEntryName{};
	string _gsVersion{};
};

