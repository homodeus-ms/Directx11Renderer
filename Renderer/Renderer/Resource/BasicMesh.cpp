#include "pch.h"
#include "BasicMesh.h"
#include "Graphics/Buffer/GeometryCreator.h"
#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Buffer/IndexBuffer.h"

BasicMesh::BasicMesh()
	: Super(EResourceType::Mesh)
{
}

BasicMesh::~BasicMesh()
{
}

void BasicMesh::Load(const wstring& path)
{

}

void BasicMesh::CreateQuad()
{
	_geometry = make_shared<Geometry<VertexUVNormalTangentData>>();
	GeometryCreator::CreateQuad(_geometry);
	CreateBuffers();
}

void BasicMesh::CreateCube()
{
	_geometry = make_shared<Geometry<VertexUVNormalTangentData>>();
	GeometryCreator::CreateCube(_geometry);
	CreateBuffers();
}

void BasicMesh::CreateGrid(int32 sizeX, int32 sizeZ)
{
	_geometry = make_shared<Geometry<VertexUVNormalTangentData>>();
	GeometryCreator::CreateGrid(_geometry, sizeX, sizeZ);
	CreateBuffers();
}

void BasicMesh::CreateSphere()
{
	_geometry = make_shared<Geometry<VertexUVNormalTangentData>>();
	GeometryCreator::CreateSphere(_geometry);
	CreateBuffers();
}

const vector<D3D11_INPUT_ELEMENT_DESC>& BasicMesh::GetInputLayoutDesc() const
{
	// TODO : Currently hardcoded to VertexUVNormalData. 
	return VertexUVNormalTangentData::descs;
}

void BasicMesh::CreateBuffers()
{
	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_geometry->GetVertices());
	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_geometry->GetIndices());
}

