#pragma once
#include "Interfaces/IConstantBuffer.h"

struct TransformData
{
	Matrix matWorld = Matrix::Identity;
	Matrix matView = Matrix::Identity;
	Matrix matProjection = Matrix::Identity;
};

template<typename T>
class ConstantBuffer : public IConstantBuffer
{
public:
	ConstantBuffer() {}
	virtual ~ConstantBuffer() {}

	ComPtr<ID3D11Buffer> GetComPtr() override { return _constantBuffer; }

	void Create() override
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DYNAMIC; // CPU_Write + GPU_Read
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(T);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = DEVICE->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
		check(hr);
	}

	void CopyData(const T& data)
	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		ZeroMemory(&subResource, sizeof(subResource));

		CONTEXT->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		::memcpy(subResource.pData, &data, sizeof(data));
		CONTEXT->Unmap(_constantBuffer.Get(), 0);
	}

private:
	ComPtr<ID3D11Buffer> _constantBuffer;
};

