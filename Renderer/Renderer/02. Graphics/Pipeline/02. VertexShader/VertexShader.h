#pragma once
#include "../ShaderBase.h"

class VertexShader : public ShaderBase
{
	using Super = ShaderBase;

public:
	VertexShader(ComPtr<ID3D11Device> device);
	~VertexShader();

	ComPtr<ID3D11VertexShader> GetComPtr() { return _vertexShader; }

	virtual void Create(const wstring& shaderPath, const string& funcName, const string& version) override;


private:
	ComPtr<ID3D11VertexShader> _vertexShader;
};

