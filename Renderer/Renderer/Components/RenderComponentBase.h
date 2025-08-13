#pragma once
#include "Component.h"
#include "Structs/EShaderStage.h"
#include "Structs/ShaderParameterTypes.h"

class MaterialBase;

class RenderComponentBase : public Component
{
	using Super = Component;

public:
	RenderComponentBase(EComponentType componentType);
	virtual ~RenderComponentBase();
	
	virtual void BeginPlay() override;
	virtual void Render();
	virtual void RenderDepthOnly(bool bForPointLight, int32 instanceCount);
	virtual void RenderDrawNormal();

	virtual vector<shared_ptr<MaterialBase>> GetMaterials() abstract;
	virtual void ChangeMaterialType(EMaterialType type) abstract;
	

protected:

	void Draw(UINT vertexCount, UINT startVertexLocation = 0);
	void DrawIndexed(UINT indexCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0);
	void DrawInstanced(UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation = 0, UINT startInstanceLocation = 0);
	void DrawIndexedInstanced(UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0, UINT startInstanceLocation = 0);

	bool _bRenderReady = false;
};

