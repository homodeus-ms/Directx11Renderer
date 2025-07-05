#pragma once

class VertexBuffer
{
public:
	VertexBuffer(ComPtr<ID3D11Device> device);
	~VertexBuffer();

	ComPtr<ID3D11Buffer> GetComPtr() const { return _vertexBuffer; }
	uint32 GetStride() const { return _stride; }
	uint32 GetOffset() const { return _offset; }
	uint32 GetCount() const { return _count; }

	template<typename T>
	void Create(const vector<T>& vertices)
	{
		_stride = sizeof(T);
		_count = static_cast<uint32>(vertices.size());

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.ByteWidth = static_cast<uint32>(sizeof(Vertex) * _count);

		D3D11_SUBRESOURCE_DATA subResourceData;
		ZeroMemory(&subResourceData, sizeof(subResourceData));
		subResourceData.pSysMem = vertices.data();    // Where is Source

		HRESULT hr = _device->CreateBuffer(&bufferDesc, &subResourceData, _vertexBuffer.GetAddressOf());
		check(hr);
	}

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;

	uint32 _stride = 0;
	uint32 _offset = 0;
	uint32 _count = 0;
};

