#pragma once

class IConstantBuffer
{
public:
	virtual ~IConstantBuffer() {}

	virtual ComPtr<ID3D11Buffer> GetComPtr() abstract;
	virtual void Create() abstract;
};

