#pragma once
#include "Component.h"
#include "Structs/EShaderStage.h"

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

protected:
	virtual void SetInputLayout() abstract;
	void SetVertexShader(shared_ptr<ShaderInfo> shaderInfo);
	void SetPixelShader(shared_ptr<ShaderInfo> shaderInfo);
	void GetDefaultStates();

	void Draw(UINT vertexCount, UINT startVertexLocation = 0);
	void DrawIndexed(UINT indexCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0);
	void DrawInstanced(UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation = 0, UINT startInstanceLocation = 0);
	void DrawIndexedInstanced(UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0, UINT startInstanceLocation = 0);


	shared_ptr<InputLayout> _inputLayout = nullptr;
	shared_ptr<VertexShader> _vertexShader;
	shared_ptr<PixelShader> _pixelShader;
	shared_ptr<PipelineState> _pipelineState;

	ComPtr<ID3D11SamplerState> _samplerState;

	bool _bRenderReady = false;

};

