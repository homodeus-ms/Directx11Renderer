#pragma once
#include "Shader.h"

class ComputeShader : public Shader
{
	using Super = Shader;

public:
	ComputeShader();
	virtual ~ComputeShader();

	ComPtr<ID3D11ComputeShader> GetComPtr() const { return _computeShader; }
	virtual void Create(const wstring& shaderPath, const string& entryName, const string& version) override;

private:
	ComPtr<ID3D11ComputeShader> _computeShader;
};

