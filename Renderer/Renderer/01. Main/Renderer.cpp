#include "pch.h"
#include "Renderer.h"
#include "02. Graphics/Graphics.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
	
}

void Renderer::BeginPlay(HWND hWnd)
{
	_hWnd = hWnd;
	_graphics = make_shared<Graphics>(_hWnd);
	_device = _graphics->GetDevice();
	_deviceContext = _graphics->GetDeviceContext();

	RenderInit();
}

void Renderer::RenderInit()
{
	CreateGeometry();
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateSRV();
	CreateVS();
	CreatePS();
	CreateInputLayout();
	CreateConstantBuffer();
}

void Renderer::Tick()
{
	_transformData.offset.x += 0.003f;
	_transformData.offset.y += 0.003f;

	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));

	_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	::memcpy(subResource.pData, &_transformData, sizeof(_transformData));
	_deviceContext->Unmap(_constantBuffer.Get(), 0);
}

void Renderer::Render()
{
	_graphics->RenderBegin();

	// Pipeline

	uint32 stride = sizeof(Vertex);
	uint32 offset = 0;

	// IA
	_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	_deviceContext->IASetInputLayout(_inputLayout.Get());
	_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VS
	_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
	_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
	// RS

	// PS
	_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
	_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
	// OM
	_deviceContext->DrawIndexed(_indices.size(), 0, 0);

	_graphics->RenderEnd();
}

void Renderer::CreateGeometry()
{
	_vertices.resize(4);

	_vertices[0].position = { -0.5f, -0.5f, 0.f };
	_vertices[0].uv = { 0.f, 1.f };
	_vertices[1].position = { -0.5f, 0.5f, 0.f };
	_vertices[1].uv = { 0.f, 0.f };
	_vertices[2].position = { 0.5f, -0.5f, 0.f };
	_vertices[2].uv = { 1.f, 1.f };
	_vertices[3].position = { 0.5f, 0.5f, 0.f };
	_vertices[3].uv = { 1.f, 0.f };
}

void Renderer::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = static_cast<uint32>(sizeof(Vertex) * _vertices.size());
	
	D3D11_SUBRESOURCE_DATA subResourceData;
	ZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = _vertices.data();    // Where is Source

	_device->CreateBuffer(&bufferDesc, &subResourceData, _vertexBuffer.GetAddressOf());
}

void Renderer::CreateIndexBuffer()
{
	_indices = { 0, 1, 2, 2, 1, 3 };

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = static_cast<uint32>(sizeof(uint32) * _indices.size());

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = _indices.data();

	_device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
}

void Renderer::CreateVS()
{
	CompileShaderAndGetBlob(L"Triangle.hlsl", "VS", "vs_5_0", _vsBlob);

	HRESULT hr = _device->CreateVertexShader(
		_vsBlob->GetBufferPointer(),
		_vsBlob->GetBufferSize(),
		nullptr,
		_vertexShader.GetAddressOf());

	check(hr);
}

void Renderer::CreatePS()
{
	CompileShaderAndGetBlob(L"Triangle.hlsl", "PS", "ps_5_0", _psBlob);

	HRESULT hr = _device->CreatePixelShader(
		_psBlob->GetBufferPointer(),
		_psBlob->GetBufferSize(),
		nullptr,
		_pixelShader.GetAddressOf());

	check(hr);
}

void Renderer::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	HRESULT hr = _device->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
	check(hr);
}

void Renderer::CreateSRV()
{
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage img;
	HRESULT hr = ::LoadFromWICFile(L"TempAssets\\Guitarist.png", WIC_FLAGS_NONE, &metadata, img);
	check(hr);

	hr = ::CreateShaderResourceView(
		_device.Get(),
		img.GetImages(),
		img.GetImageCount(),
		metadata,
		_shaderResourceView.GetAddressOf());
	check(hr);
}

void Renderer::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(TransformData);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = _device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
	check(hr);
}

void Renderer::CompileShaderAndGetBlob(const wstring& fileName, const string& funcName, const string& version, ComPtr<ID3DBlob>& OUTBlob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	HRESULT hr = ::D3DCompileFromFile(
		fileName.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		funcName.c_str(),
		version.c_str(),
		compileFlag,
		0,
		OUTBlob.GetAddressOf(),
		nullptr);

	check(hr);
}
