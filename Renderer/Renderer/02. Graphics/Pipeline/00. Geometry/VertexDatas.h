#pragma once

struct VertexUVData
{
	Vec3 position{};
	Vec2 uv{};

	static vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

struct VertexColorData
{
	Vec3 position{};
	Color color{};

	static vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

