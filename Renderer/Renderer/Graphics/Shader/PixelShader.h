#pragma once
#include "Shader.h"

class PixelShader : public Shader
{
public:
	PixelShader();
	virtual ~PixelShader();

	ComPtr<ID3D11PixelShader> GetComPtr() const { return _pixelShader; }
	virtual void Create(const wstring& shaderPath, const string& entryName, const string& version) override;


private:

	ComPtr<ID3D11PixelShader> _pixelShader;
};

