#include "pch.h"
#include "ImportedMeshData.h"
#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Buffer/IndexBuffer.h"

void ImportedStaticMesh::CreateBuffers()
{
	vertexBuffer = make_shared<VertexBuffer>();
	vertexBuffer->Create(geometry->GetVertices());
	indexBuffer = make_shared<IndexBuffer>();
	indexBuffer->Create(geometry->GetIndices());
}
