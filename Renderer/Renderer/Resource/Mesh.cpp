#include "pch.h"
#include "Mesh.h"
#include "Graphics/Buffer/GeometryCreator.h"
#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Buffer/IndexBuffer.h"

Mesh::Mesh()
	: Super(EResourceType::Mesh)
{
}

Mesh::~Mesh()
{
}

void Mesh::Load(const wstring& path)
{

}

void Mesh::CreateQuad()
{
	_geometry = make_shared<Geometry<VertexUVNormalTangentData>>();
	GeometryCreator::CreateQuad(_geometry);
	CreateBuffers();
}

void Mesh::CreateCube()
{
	_geometry = make_shared<Geometry<VertexUVNormalTangentData>>();
	GeometryCreator::CreateCube(_geometry);
	CreateBuffers();
}

void Mesh::CreateGrid(int32 sizeX, int32 sizeZ)
{
	_geometry = make_shared<Geometry<VertexUVNormalTangentData>>();
	GeometryCreator::CreateGrid(_geometry, sizeX, sizeZ);
	CreateBuffers();
}

void Mesh::CreateSphere()
{
	_geometry = make_shared<Geometry<VertexUVNormalTangentData>>();
	GeometryCreator::CreateSphere(_geometry);
	CreateBuffers();
}

const vector<D3D11_INPUT_ELEMENT_DESC>& Mesh::GetInputLayoutDesc() const
{
	// TODO : Currently hardcoded to VertexUVNormalData. 
	return VertexUVNormalTangentData::descs;
}

void Mesh::CreateBuffers()
{
	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_geometry->GetVertices());
	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_geometry->GetIndices());
}

