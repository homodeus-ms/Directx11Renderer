#pragma once
#include "RenderComponentBase.h"

class BasicMesh;
class MaterialBase;

class BasicMeshRenderer : public RenderComponentBase
{
	using Super = RenderComponentBase;

public:
	BasicMeshRenderer();
	virtual ~BasicMeshRenderer();

	virtual vector<shared_ptr<MaterialBase>> GetMaterials() override;
	void SetBasicMesh(const shared_ptr<BasicMesh>& mesh);
	void SetBasicMaterial(const shared_ptr<MaterialBase>& material);
	virtual void ChangeMaterialType(EMaterialType type) override;
	
	virtual void Construct() override;
	virtual void BeginPlay() override;
	virtual void Render() override;
	virtual void RenderDepthOnly(bool bForPointLight, int32 instanceCount) override;
	virtual void RenderDrawNormal() override;
private:
	shared_ptr<BasicMesh> _basicMesh;
	shared_ptr<MaterialBase> _basicMaterial;
};

