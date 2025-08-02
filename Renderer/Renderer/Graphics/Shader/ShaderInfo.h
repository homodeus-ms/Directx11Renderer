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

	ShaderInfo(const wstring& shaderName, string vsEntryName = "VS", string psEntryName = "PS", EShaderVersion version = EShaderVersion::Ver5_0);
	ShaderInfo(const wstring& vsShaderName, const wstring& psShaderName, string vsEntryName = "VS", string psEntryName = "PS", EShaderVersion version = EShaderVersion::Ver5_0);
	~ShaderInfo();

	// TEMP : VS가 있는 셰이더 파일에 같은 사용한다고 가정
	void AddGSShaderInfo(string gsEntryName = "GS", EShaderVersion version = EShaderVersion::Ver5_0);

	bool IsValidVersion(EShaderVersion version)
	{
		return version == EShaderVersion::Ver5_0 || version == EShaderVersion::Ver5_1;
	}

	wstring _shaderPath{};
	wstring _vsShaderPath{};
	wstring _psShaderPath{};
	string _vsEntryName{}; 
	string _psEntryName{};
	string _vsVersion{};
	string _psVersion{};
	string _gsEntryName{};
	string _gsVersion{};
};

