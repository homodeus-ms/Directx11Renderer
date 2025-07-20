#pragma once

struct ShadowMapResources
{
	shared_ptr<class InputLayout> inputLayout{};
	shared_ptr<class VertexShader> vertexShader{};
	shared_ptr<class PixelShader> pixelShader{};
	ComPtr<ID3D11RasterizerState> rasterizerState{};
	ComPtr<ID3D11DepthStencilState> DepthStencilState{};
};

