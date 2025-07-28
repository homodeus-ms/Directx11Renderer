#pragma once
#include "StaticMesh.h"

class SkeletalMesh : public StaticMesh
{
	using Super = StaticMesh;

public:
	SkeletalMesh();
	virtual ~SkeletalMesh();

	virtual void BindMeshElements() override;

private:
	
};

