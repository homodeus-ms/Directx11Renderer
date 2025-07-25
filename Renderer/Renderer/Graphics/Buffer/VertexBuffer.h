#pragma once

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	ComPtr<ID3D11Buffer> GetComPtr() const { return _vertexBuffer; }
	ComPtr<ID3D11Buffer> GetPosOnlyBuffer() const { return _posOnlyVertexBuffer; }
	uint32 GetStride() const { return _stride; }
	uint32 GetOffset() const { return _offset; }
	uint32 GetCount() const { return _count; }

	template<typename T>
	void Create(const vector<T>& vertices)
	{
		_stride = sizeof(T);
		_count = static_cast<uint32>(vertices.size());

		{
			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.ByteWidth = (uint32)(_stride * _count);

			D3D11_SUBRESOURCE_DATA data;
			ZeroMemory(&data, sizeof(data));
			data.pSysMem = vertices.data();

			HRESULT hr = DEVICE->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
			check(hr);
		}


		// PosOnlyBuffer
		vector<VertexData> posOnlyVector;
		posOnlyVector.reserve(_count);

		static_assert(offsetof(T, position) == 0, "T must have Vec3 position at offset 0");

		for (auto& vertex : vertices)
		{
			VertexData pos;
			::memcpy(&pos, &vertex, sizeof(Vec3));
			posOnlyVector.push_back(pos);
		}

		uint32 posOnlyStride = sizeof(VertexData);

		{
			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.ByteWidth = (uint32)(posOnlyStride * _count);

			D3D11_SUBRESOURCE_DATA data;
			ZeroMemory(&data, sizeof(data));
			data.pSysMem = posOnlyVector.data();

			HRESULT hr = DEVICE->CreateBuffer(&desc, &data, _posOnlyVertexBuffer.GetAddressOf());
			check(hr);
		}
	}

private:
	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _posOnlyVertexBuffer;

	uint32 _stride = 0;
	uint32 _offset = 0;
	uint32 _count = 0;
};

