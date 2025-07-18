#pragma once

#include "Graphics/Buffer/Geometry.h"
#include "ResourceBase.h"

class VertexBuffer;
class IndexBuffer;

class BasicMesh : public ResourceBase
{
	using Super = ResourceBase;

public:
	BasicMesh();
	virtual ~BasicMesh();

	virtual void Load(const wstring& path) override;

	// TODO : Currently Hardcoded using VertexUVNormalTangent Only
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
	shared_ptr<Geometry<VertexUVNormalTangentData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
};

