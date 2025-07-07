#pragma once
#include "Shader.h"

class VertexShader : public Shader
{
public:
	VertexShader();
	virtual ~VertexShader();

	ComPtr<ID3D11VertexShader> GetComPtr() const { return _vertexShader; }
	virtual void Create(const wstring& shaderPath, const string& entryName, const string& version) override;


private:

	ComPtr<ID3D11VertexShader> _vertexShader;
};

