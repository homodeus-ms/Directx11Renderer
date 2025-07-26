#pragma once
#include "Component.h"
#include "Structs/EShaderStage.h"
#include "Structs/ShaderParameterTypes.h"
#include "Graphics/RenderPass/ShadowMapResources.h"

class InputLayout;
class VertexShader;
class PixelShader;
class PipelineState;
class BasicMesh;
class Material;
struct ShaderInfo;

class RenderComponentBase : public Component
{
	using Super = Component;

public:
	RenderComponentBase(EComponentType componentType);
	virtual ~RenderComponentBase();
	
	virtual void BeginPlay() override;
	virtual void Render();
	virtual void RenderDepthOnly(bool bForPointLight, int32 instanceCount);

	virtual vector<shared_ptr<Material>> GetMaterials() abstract;
	virtual void ChangeMaterialType(EMaterialType type) abstract;

protected:
	virtual void SetInputLayout() abstract;
	void SetVertexShader(shared_ptr<ShaderInfo> shaderInfo);
	void SetPixelShader(shared_ptr<ShaderInfo> shaderInfo);
	void GetDefaultStates();

	void Draw(UINT vertexCount, UINT startVertexLocation = 0);
	void DrawIndexed(UINT indexCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0);
	void DrawInstanced(UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation = 0, UINT startInstanceLocation = 0);
	void DrawIndexedInstanced(UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0, UINT startInstanceLocation = 0);

	void ClearGeometryShader();

	shared_ptr<InputLayout> _inputLayout = nullptr;
	shared_ptr<VertexShader> _vertexShader;
	shared_ptr<PixelShader> _pixelShader;
	shared_ptr<PipelineState> _pipelineState;
	ComPtr<ID3D11SamplerState> _samplerState;

	bool _bRenderReady = false;

	// ShadowMap
	ShadowMapResources _shadowMapResources;

	// TEMP
	ComPtr<ID3D11RasterizerState> _tempRS;
	ComPtr<ID3D11SamplerState> _tempSS;
};

