#pragma once
#include "RenderComponentBase.h"

class StaticMesh;

class StaticMeshRenderer : public RenderComponentBase
{
	using Super = RenderComponentBase;

public:
	StaticMeshRenderer();
	virtual ~StaticMeshRenderer();
	
	virtual vector<shared_ptr<MaterialBase>> GetMaterials() override;
	void SetStaticMesh(const shared_ptr<StaticMesh>& staticMesh) { _staticMesh = staticMesh; }
	
	virtual void ChangeMaterialType(EMaterialType type) override;

	virtual void Construct() override;
	virtual void BeginPlay() override;
	virtual void Render() override;
	virtual void RenderDepthOnly(bool bForPointLight, int32 instanceCount) override;
	virtual void RenderDrawNormal() override;
private:
	shared_ptr<StaticMesh> _staticMesh;
};

