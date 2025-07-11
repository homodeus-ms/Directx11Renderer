#pragma once

#include "Graphics/Buffer/Geometry.h"
#include "ResourceBase.h"

class VertexBuffer;
class IndexBuffer;

class Mesh : public ResourceBase
{
	using Super = ResourceBase;

public:
	Mesh();
	virtual ~Mesh();

	virtual void Load(const wstring& path) override;

	// TODO : Currently Hardcoded using VertexUVNormal Only
	void CreateQuad();
	void CreateCube();
	void CreateGrid(int32 sizeX, int32 sizeZ);
	void CreateSphere();

	const vector<D3D11_INPUT_ELEMENT_DESC>& GetInputLayoutDesc() const;
	shared_ptr<VertexBuffer> GetVertexBuffer() const { return _vertexBuffer; }
	shared_ptr<IndexBuffer> GetIndexBuffer() const { return _indexBuffer; }
	const vector<D3D11_INPUT_ELEMENT_DESC>& GetInputDesc() const { _geometry->GetInputDesc(); }

private:
	void CreateBuffers();

	// Mesh
	shared_ptr<Geometry<VertexUVNormalData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
};

