#pragma once

enum { SHADOW_MAP_SIZE = 2048 };

struct ShadowMapResources
{
	shared_ptr<class InputLayout> inputLayout{};

	shared_ptr<class VertexShader> defaultVertexShader{};
	shared_ptr<class GeometryShader> defaultGeometryShader{};
	shared_ptr<class PixelShader> defaultPixelShader{};

	shared_ptr<class VertexShader> pointLightVertexShader{};
	shared_ptr<class GeometryShader> pointLightGeometryShader{};
	shared_ptr<class PixelShader> pointLightPixelShader{};

	ComPtr<ID3D11RasterizerState> rasterizerState{};
	ComPtr<ID3D11DepthStencilState> depthStencilState{};
};

