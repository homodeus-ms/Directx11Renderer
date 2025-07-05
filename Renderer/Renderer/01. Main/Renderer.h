#pragma once

class Graphics;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void BeginPlay(HWND hWnd);
	void Tick();
	void Render();

	void RenderInit();

private:

	void CreateGeometry();
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateVS();
	void CreatePS();
	void CreateInputLayout();
	void CreateSRV();
	void CreateConstantBuffer();

	void CompileShaderAndGetBlob(const wstring& fileName, const string& funcName, const string& version, ComPtr<ID3DBlob>& OUTBlob);

private:
	HWND _hWnd{};
	shared_ptr<Graphics> _graphics;
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;

	// Geometry
	vector<Vertex> _vertices;
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;
	vector<uint32> _indices;
	ComPtr<ID3D11Buffer> _indexBuffer = nullptr;

	// VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;

	// PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;

	// SRV
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView = nullptr;

	// CBuffer
	TransformData _transformData;
	ComPtr<ID3D11Buffer> _constantBuffer = nullptr;
};
