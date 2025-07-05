#pragma once

class IndexBuffer
{
public:
	IndexBuffer(ComPtr<ID3D11Device> device);
	~IndexBuffer();

	ComPtr<ID3D11Buffer> GetComPtr() const { return _indexBuffer; }
	void Create(const vector<uint32>& indices);

	uint32 GetStride() const { return _stride; }
	uint32 GetOffset() const { return _offset; }
	uint32 GetCount() const { return _count; }

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11Buffer> _indexBuffer = nullptr;
	
	uint32 _stride = 0;
	uint32 _offset = 0;
	uint32 _count = 0;
};

