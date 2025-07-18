#pragma once
#include "RenderComponentBase.h"

class BasicMesh;
class Material;

class BasicMeshRenderer : public RenderComponentBase
{
	using Super = RenderComponentBase;

public:
	BasicMeshRenderer();
	virtual ~BasicMeshRenderer();

	virtual void SetInputLayout() override;
	void SetBasicMesh(const shared_ptr<BasicMesh>& mesh);
	void SetBasicMaterial(const shared_ptr<Material>& material);
	
	virtual void Construct() override;
	virtual void BeginPlay() override;
	virtual void Render() override;

private:
	shared_ptr<BasicMesh> _basicMesh;
	shared_ptr<Material> _basicMaterial;
};

