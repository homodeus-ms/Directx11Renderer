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

struct VertexUVNormalTangentData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };
	Vec3 normal = { 0, 0, 0 };
	Vec3 tangent = { 0, 0, 0 };

	static vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

struct VertexUVNormalTangentBlendData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };
	Vec3 normal = { 0, 0, 0 };
	Vec3 tangent = { 0, 0, 0 };
	uint32 blendIndices[4]{};
	Vec4 blendWeights = { 0, 0, 0, 0 };

	static vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

using StaticMeshVertexType = VertexUVNormalTangentBlendData;
using SkeletalMeshVertexType = VertexUVNormalTangentBlendData;