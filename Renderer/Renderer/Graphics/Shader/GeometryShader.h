#pragma once
#include "Shader.h"

class GeometryShader : public Shader
{
	using Super = Shader;

public:
	GeometryShader();
	virtual ~GeometryShader();

	ComPtr<ID3D11GeometryShader> GetComPtr() const { return _geometryShader; }
	virtual void Create(const wstring& shaderPath, const string& entryName, const string& version) override;

private:
	ComPtr<ID3D11GeometryShader> _geometryShader;
};

