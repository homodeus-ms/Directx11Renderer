#pragma once
#include "BasicMesh.h"

class CubeMapBasicMesh : public BasicMesh
{
	using Super = BasicMesh;

public:
	CubeMapBasicMesh() {}
	virtual ~CubeMapBasicMesh() {}

	virtual void CreateCubeMap() override;

	virtual const vector<D3D11_INPUT_ELEMENT_DESC>& GetInputLayoutDesc() const override { return _geometry->GetInputDesc(); }

protected:
	enum { DEFAULT_CUBE_MAP_SIZE = 100 };
	virtual void CreateBuffers() override;

private:
	shared_ptr<Geometry<VertexData>> _geometry;


};

