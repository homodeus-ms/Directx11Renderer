#pragma once
#include "Component.h"
#include "Structs/MeshStructDatas.h"
#include "Structs/EShaderStage.h"
#include "Graphics/Buffer/ConstantBuffer.h"

class InputLayout;
class VertexShader;
class PixelShader;
class PipelineState;

class RenderComponent : public Component
{
	using Super = Component;

public:
	RenderComponent();
	virtual ~RenderComponent();

	void SetStaticMeshInfo(const StaticMeshInfo& info);

	virtual void Construct() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;
	void Render();
	

private:

	void UpdatePipeline();
	void SetInputLayout();
	void SetVertexShader();
	void SetPixelShader();
	void GetDefaultStates();
	void SetVertexBuffer();
	void SetIndexBuffer();

	/*template<typename T>
	void SetConstantBuffer(uint32 slot, EShaderStage stage, shared_ptr<ConstantBuffer<T>> buffer)
	{
		
	}*/
	// TEMP

	void SetTexture(uint32 slot, EShaderStage stage, shared_ptr<Texture> texture);

	void Draw(UINT vertexCount, UINT startVertexLocation = 0);
	void DrawIndexed(UINT indexCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0);
	void DrawInstanced(UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation = 0, UINT startInstanceLocation = 0);
	void DrawIndexedInstanced(UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0, UINT startInstanceLocation = 0);
	

	bool bHasInfo = false;
	shared_ptr<InputLayout> _inputLayout = nullptr;

	StaticMeshInfo _staticMeshInfo;
	shared_ptr<VertexShader> _vertexShader;
	shared_ptr<PixelShader> _pixelShader;

	shared_ptr<PipelineState> _pipelineState;

	// TEMP ConstBuffer
	bool _transformBufferDirty = false;
	shared_ptr<ConstantBuffer<TransformData>> _transformBuffer;
	TransformData _transformData{};
};

