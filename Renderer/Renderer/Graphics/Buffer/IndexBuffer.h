#pragma once

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	ComPtr<ID3D11Buffer> GetComPtr() const { return _indexBuffer; }
	uint32 GetStride() const { return _stride; }
	uint32 GetOffset() const { return _offset; }
	uint32 GetCount() const { return _count; }

	void Create(const vector<uint32>& indices);

private:
	ComPtr<ID3D11Buffer> _indexBuffer;

	uint32 _stride = 0;
	uint32 _offset = 0;
	uint32 _count = 0;
};

