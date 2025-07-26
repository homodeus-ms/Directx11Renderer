#pragma once
#include "RenderComponentBase.h"

class StaticMesh;

class StaticMeshRenderer : public RenderComponentBase
{
	using Super = RenderComponentBase;

public:
	StaticMeshRenderer();
	virtual ~StaticMeshRenderer();

	virtual void SetInputLayout() override;
	virtual vector<shared_ptr<Material>> GetMaterials() override;
	void SetStaticMesh(const shared_ptr<StaticMesh>& staticMesh) { _staticMesh = staticMesh; }
	void SetShaderInfo(shared_ptr<ShaderInfo> shaderInfo);
	virtual void ChangeMaterialType(EMaterialType type) override;

	virtual void Construct() override;
	virtual void BeginPlay() override;
	virtual void Render() override;
	virtual void RenderDepthOnly(bool bForPointLight, int32 instanceCount) override;

private:
	shared_ptr<StaticMesh> _staticMesh;
	shared_ptr<ShaderInfo> _shaderInfo;

};

