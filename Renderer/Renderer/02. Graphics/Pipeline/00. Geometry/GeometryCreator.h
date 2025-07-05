#pragma once

#include "Geometry.h"
#include "VertexDatas.h"

class GeometryCreator
{
	static void CreateSquare(shared_ptr<Geometry<VertexColorData>> OUT geometry, Color color);
	static void CreateSquare(shared_ptr<Geometry<VertexUVData>> OUT geometry);
};

