#pragma once

#include "../ShaderBase.h"

class PixelShader : public ShaderBase
{
	using Super = ShaderBase;

public:
	PixelShader(ComPtr<ID3D11Device> device);
	~PixelShader();

	ComPtr<ID3D11PixelShader> GetComPtr() const { return _pixelShader; }

	virtual void Create(const wstring& shaderPath, const string& funcName, const string& version) override;

private:
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
};

