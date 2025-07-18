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
	void SetStaticMesh(const shared_ptr<StaticMesh>& staticMesh) { _staticMesh = staticMesh; }
	void SetShaderInfo(shared_ptr<ShaderInfo> shaderInfo);

	virtual void Construct() override;
	virtual void BeginPlay() override;
	virtual void Render() override;


private:
	shared_ptr<StaticMesh> _staticMesh;
	shared_ptr<ShaderInfo> _shaderInfo;

};

