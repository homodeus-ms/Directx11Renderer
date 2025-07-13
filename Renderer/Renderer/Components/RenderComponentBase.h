#pragma once
#include "Component.h"
#include "Structs/EShaderStage.h"

class InputLayout;
class VertexShader;
class PixelShader;
class PipelineState;
class Mesh;
class Material;
struct ShaderInfo;

class RenderComponentBase : public Component
{
	using Super = Component;

public:
	RenderComponentBase(EComponentType componentType);
	virtual ~RenderComponentBase();

	void SetMesh(const shared_ptr<Mesh>& mesh);
	void SetMaterial(const shared_ptr<Material>& material);
	
	virtual void BeginPlay() override;
	virtual void Render();

protected:
	void SetInputLayout();
	void SetVertexShader(shared_ptr<ShaderInfo> shaderInfo);
	void SetPixelShader(shared_ptr<ShaderInfo> shaderInfo);
	void GetDefaultStates();
	void SetVertexBuffer();
	void SetIndexBuffer();

	void Draw(UINT vertexCount, UINT startVertexLocation = 0);
	void DrawIndexed(UINT indexCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0);
	void DrawInstanced(UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation = 0, UINT startInstanceLocation = 0);
	void DrawIndexedInstanced(UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0, UINT startInstanceLocation = 0);
	
	bool _bHasMesh = false;
	bool _bHasMaterial = false;

	shared_ptr<Mesh> _mesh;
	shared_ptr<Material> _material;

	shared_ptr<InputLayout> _inputLayout = nullptr;
	shared_ptr<VertexShader> _vertexShader;
	shared_ptr<PixelShader> _pixelShader;

	shared_ptr<PipelineState> _pipelineState;

};

