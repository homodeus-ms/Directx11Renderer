#include "pch.h"
#include "VertexUVBasicMesh.h"
#include "Graphics/Buffer/GeometryCreator.h"
#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Buffer/IndexBuffer.h"

void VertexUVBasicMesh::CreateQuad()
{
	_geometry = make_shared<Geometry<VertexUVData>>();
	GeometryCreator::CreateQuad(_geometry);
	CreateBuffers();
}

void VertexUVBasicMesh::CreateCube()
{
	_geometry = make_shared<Geometry<VertexUVData>>();
	GeometryCreator::CreateCube(_geometry);
	CreateBuffers();
}

void VertexUVBasicMesh::CreateGrid(int32 sizeX, int32 sizeZ)
{
	_geometry = make_shared<Geometry<VertexUVData>>();
	GeometryCreator::CreateGrid(_geometry, sizeX, sizeZ);
	CreateBuffers();
}

void VertexUVBasicMesh::CreateSphere()
{
	_geometry = make_shared<Geometry<VertexUVData>>();
	GeometryCreator::CreateSphere(_geometry);
	CreateBuffers();
}

void VertexUVBasicMesh::CreateBuffers()
{
	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_geometry->GetVertices());
	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_geometry->GetIndices());
}
