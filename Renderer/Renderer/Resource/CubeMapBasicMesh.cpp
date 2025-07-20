#include "pch.h"
#include "CubeMapBasicMesh.h"
#include "Graphics/Buffer/GeometryCreator.h"
#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Buffer/IndexBuffer.h"

void CubeMapBasicMesh::CreateCubeMap()
{
	_geometry = make_shared<Geometry<VertexData>>();
	GeometryCreator::CreateCubeMap(_geometry, DEFAULT_CUBE_MAP_SIZE);
	CreateBuffers();
}

void CubeMapBasicMesh::CreateBuffers()
{
	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_geometry->GetVertices());
	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_geometry->GetIndices());
}
