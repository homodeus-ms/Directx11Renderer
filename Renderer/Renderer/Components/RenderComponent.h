#pragma once
#include "Component.h"
#include "Structs/MeshStructDatas.h"
#include "Structs/EShaderStage.h"

class InputLayout;
class VertexShader;
class PixelShader;
class PipelineState;
class Mesh;
class Material;

class RenderComponent : public Component
{
	using Super = Component;

public:
	RenderComponent();
	virtual ~RenderComponent();

	//void SetStaticMeshInfo(const StaticMeshInfo& info);
	void SetMesh(const shared_ptr<Mesh>& mesh);
	void SetMaterial(const shared_ptr<Material>& material);
	
	virtual void BeginPlay() override;
	virtual void Render();

private:

	void UpdatePipeline();
	void SetInputLayout();
	void SetVertexShader();
	void SetPixelShader();
	void GetDefaultStates();
	void SetVertexBuffer();
	void SetIndexBuffer();

	void Draw(UINT vertexCount, UINT startVertexLocation = 0);
	void DrawIndexed(UINT indexCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0);
	void DrawInstanced(UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation = 0, UINT startInstanceLocation = 0);
	void DrawIndexedInstanced(UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0, UINT startInstanceLocation = 0);
	

	bool _bHasMesh = false;
	bool _bHasMaterial = false;

	//StaticMeshInfo _staticMeshInfo;

	//vector<shared_ptr<Mesh>> _meshes;
	//vector<shared_ptr<Material>> _materials;

	shared_ptr<Mesh> _mesh;
	shared_ptr<Material> _material;

	shared_ptr<InputLayout> _inputLayout = nullptr;
	shared_ptr<VertexShader> _vertexShader;
	shared_ptr<PixelShader> _pixelShader;

	shared_ptr<PipelineState> _pipelineState;

};

