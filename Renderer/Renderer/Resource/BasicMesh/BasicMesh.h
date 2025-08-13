#pragma once

#include "Graphics/Buffer/Geometry.h"
#include "Resource/MeshBase.h"

class VertexBuffer;
class IndexBuffer;

class BasicMesh : public MeshBase
{
	using Super = MeshBase;

public:
	BasicMesh() : Super(EResourceType::BasicMesh) {}
	virtual ~BasicMesh() {};

	virtual void CreateQuad() {}
	virtual void CreateCube() {}
	virtual void CreateGrid(int32 sizeX, int32 sizeZ) {}
	virtual void CreateSphere() {}
	virtual void CreateSphere(float radius, int numSlices, int numStacks, Vec2 texScale) {}
	virtual void CreateCubeMap() {}
	virtual void CreateSquareRoom(int32 size = 80) {}

	virtual void Load(const wstring& path) override {};

	virtual const vector<D3D11_INPUT_ELEMENT_DESC>& GetInputLayoutDesc() const abstract;
	shared_ptr<VertexBuffer> GetVertexBuffer() const { return _vertexBuffer; }
	shared_ptr<IndexBuffer> GetIndexBuffer() const { return _indexBuffer; }

	

protected:
	virtual void CreateBuffers() abstract;

	// Mesh
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;

};

