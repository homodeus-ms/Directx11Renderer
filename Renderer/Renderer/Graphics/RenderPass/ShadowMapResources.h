#pragma once

enum { SHADOW_MAP_SIZE = 2048 };

struct ShadowMapResources
{
	shared_ptr<class InputLayout> inputLayout{};

	shared_ptr<class VertexShader> defaultVertexShader{};
	shared_ptr<class VertexShader> pointLightVertexShader{};
	shared_ptr<class GeometryShader> pointLightGeometryShader{};
	shared_ptr<class PixelShader> pointLightPixelShader{};
};

