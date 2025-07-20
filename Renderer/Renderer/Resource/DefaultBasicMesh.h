#pragma once
#include "BasicMesh.h"
class DefaultBasicMesh : public BasicMesh
{
	using Super = BasicMesh;

public:
	DefaultBasicMesh() : Super() {}
	virtual ~DefaultBasicMesh() {}

	virtual void CreateQuad() override;
	virtual void CreateCube() override;
	virtual void CreateGrid(int32 sizeX, int32 sizeZ) override;
	virtual void CreateSphere() override;

	virtual const vector<D3D11_INPUT_ELEMENT_DESC>& GetInputLayoutDesc() const override { return _geometry->GetInputDesc(); }

protected:
	virtual void CreateBuffers() override;

private:
	shared_ptr<Geometry<VertexUVNormalTangentData>> _geometry;
};

