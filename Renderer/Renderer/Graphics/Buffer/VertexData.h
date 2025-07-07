#pragma once

struct VertexData
{
	Vec3 position = { 0, 0, 0 };

	static vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

struct VertexUVData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };

	static vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

struct VertexColorData
{
	Vec3 position = { 0, 0, 0 };
	Color color = { 0, 0, 0, 0 };

	static vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

struct VertexUVNormalData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };
	Vec3 normal = { 0, 0, 0 };

	static vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

