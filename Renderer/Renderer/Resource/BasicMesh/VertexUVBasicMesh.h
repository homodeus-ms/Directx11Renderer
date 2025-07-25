#pragma once
#include "BasicMesh.h"

class VertexUVBasicMesh : public BasicMesh
{
	using Super = BasicMesh;

public:
	VertexUVBasicMesh() {}
	virtual ~VertexUVBasicMesh() {}

	virtual void CreateQuad() override;
	virtual void CreateCube() override;
	virtual void CreateGrid(int32 sizeX, int32 sizeZ) override;
	virtual void CreateSphere() override;

	virtual const vector<D3D11_INPUT_ELEMENT_DESC>& GetInputLayoutDesc() const override { return _geometry->GetInputDesc(); }

protected:
	
	virtual void CreateBuffers() override;

private:
	shared_ptr<Geometry<VertexUVData>> _geometry;
};

