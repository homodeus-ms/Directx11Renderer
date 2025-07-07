#pragma once

class Shader
{
public:
	Shader();
	virtual ~Shader();

	virtual void Create(const wstring& shaderPath, const string& entryName, const string& version) abstract;

	ComPtr<ID3DBlob> GetBlob() { return _blob; }

protected:
	void LoadShaderFromFile(const wstring& shaderPath, const string& entryName, const string& version);
	ComPtr<ID3DBlob> _blob;

};

