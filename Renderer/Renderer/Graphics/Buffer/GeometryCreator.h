#pragma once
#include "Geometry.h"
#include "VertexData.h"

class GeometryCreator
{
public:
	static void CreateQuad(shared_ptr<Geometry<VertexColorData>> geometry, Color color);

	static void CreateQuad(shared_ptr<Geometry<VertexUVData>> geometry);
	static void CreateCube(shared_ptr<Geometry<VertexUVData>> geometry);
	static void CreateSphere(shared_ptr<Geometry<VertexUVData>> geometry);
	static void CreateGrid(shared_ptr<Geometry<VertexUVData>> geometry, int32 sizeX, int32 sizeZ);

	static void CreateQuad(shared_ptr<Geometry<VertexUVNormalData>> geometry);
	static void CreateCube(shared_ptr<Geometry<VertexUVNormalData>> geometry);
	static void CreateGrid(shared_ptr<Geometry<VertexUVNormalData>> geometry, int32 sizeX, int32 sizeZ);
	static void CreateSphere(shared_ptr<Geometry<VertexUVNormalData>> geometry);
};

