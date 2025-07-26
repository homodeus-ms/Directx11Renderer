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
	virtual vector<shared_ptr<Material>> GetMaterials() override;
	void SetBasicMesh(const shared_ptr<BasicMesh>& mesh);
	void SetBasicMaterial(const shared_ptr<Material>& material);
	virtual void ChangeMaterialType(EMaterialType type) override;
	
	virtual void Construct() override;
	virtual void BeginPlay() override;
	virtual void Render() override;
	virtual void RenderDepthOnly(bool bForPointLight, int32 instanceCount) override;

private:
	shared_ptr<BasicMesh> _basicMesh;
	shared_ptr<Material> _basicMaterial;
};

