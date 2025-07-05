#pragma once

#include <cstdint>

enum class ShaderStage : uint8
{
	None = 0,
	VS = 1 << 0,
	PS = 1 << 1,
	Both = VS | PS
};

inline ShaderStage operator|(ShaderStage a, ShaderStage b)
{
	return static_cast<ShaderStage>(static_cast<uint8>(a) | static_cast<uint8>(b));
}

inline ShaderStage operator&(ShaderStage a, ShaderStage b)
{
	return static_cast<ShaderStage>(static_cast<uint8>(a) & static_cast<uint8>(b));
}


class ShaderBase
{
public:
	ShaderBase(ComPtr<ID3D11Device> device);
	virtual ~ShaderBase();

	virtual void Create(const wstring& shaderPath, const string& funcName, const string& version) abstract;

	ComPtr<ID3DBlob> GetBlob() { return _blob; }

protected:
	void CompileShaderAndSetBlob(const wstring& shaderPath, const string& funcName, const string& version);

	wstring _shaderPath;
	string _funcName;
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3DBlob> _blob;
};

