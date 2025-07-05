#include "pch.h"
#include "GeometryCreator.h"

void GeometryCreator::CreateSquare(shared_ptr<Geometry<VertexColorData>>OUT geometry, Color color)
{
	vector<VertexColorData> vertices;
	vertices.resize(4);
	vertices[0].position = { -0.5f, -0.5f, 0.f };
	vertices[0].color = color;
	vertices[1].position = { -0.5f, 0.5f, 0.f };
	vertices[1].color = color;
	vertices[2].position = { 0.5f, -0.5f, 0.f };
	vertices[2].color = color;
	vertices[3].position = { 0.5f, 0.5f, 0.f };
	vertices[3].color = color;

	vector<uint32> indices = { 0, 1, 2, 2, 1, 3 };

	geometry->SetVertices(vertices);
	geometry->SetIndicies(indices);
}

void GeometryCreator::CreateSquare(shared_ptr<Geometry<VertexUVData>>OUT geometry)
{
	vector<VertexUVData> vertices;
	vertices.resize(4);
	vertices[0].position = { -0.5f, -0.5f, 0.f };
	vertices[0].uv = { 0.f, 1.f };
	vertices[1].position = { -0.5f, 0.5f, 0.f };
	vertices[1].uv = { 0.f, 0.f };
	vertices[2].position = { 0.5f, -0.5f, 0.f };
	vertices[2].uv = { 1.f, 1.f };
	vertices[3].position = { 0.5f, 0.5f, 0.f };
	vertices[3].uv = { 1.f, 0.f };

	vector<uint32> indices = { 0, 1, 2, 2, 1, 3 };

	geometry->SetVertices(vertices);
	geometry->SetIndicies(indices);
}
