#include "pch.h"
#include "DefaultBasicMesh.h"
#include "Graphics/Buffer/GeometryCreator.h"
#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Buffer/IndexBuffer.h"

void DefaultBasicMesh::CreateQuad()
{
	_geometry = make_shared<Geometry<VertexUVNormalTangentData>>();
	GeometryCreator::CreateQuad(_geometry);
	CreateBuffers();
}

void DefaultBasicMesh::CreateCube()
{
	_geometry = make_shared<Geometry<VertexUVNormalTangentData>>();
	GeometryCreator::CreateCube(_geometry);
	CreateBuffers();
}

void DefaultBasicMesh::CreateGrid(int32 sizeX, int32 sizeZ)
{
	_geometry = make_shared<Geometry<VertexUVNormalTangentData>>();
	GeometryCreator::CreateGrid(_geometry, sizeX, sizeZ);
	CreateBuffers();
}

void DefaultBasicMesh::CreateSphere()
{
	_geometry = make_shared<Geometry<VertexUVNormalTangentData>>();
	GeometryCreator::CreateSphere(_geometry);
	CreateBuffers();
}

void DefaultBasicMesh::CreateSquareRoom(int32 size)
{
	_geometry = make_shared<Geometry<VertexUVNormalTangentData>>();
	GeometryCreator::CreateSquareRoom(_geometry, size);
	CreateBuffers();
}

void DefaultBasicMesh::CreateBuffers()
{
	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_geometry->GetVertices());
	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_geometry->GetIndices());
}