#pragma once
#include "Components/Component.h"
#include "Structs/MeshStructDatas.h"
#include "Graphics/Buffer/ConstantBuffer.h"
#include "Structs/EShaderStage.h"

class Transform;
class Mesh;
class RenderComponent;

class Actor : public enable_shared_from_this<Actor>
{
public:
	Actor();
	virtual ~Actor();

	virtual void Construct();
	virtual void BeginPlay();
	virtual void Tick();
	virtual void LateTick();
	virtual void FixedTick();

	void Render();

	// Component
	shared_ptr<Component> GetFixedComponent(ComponentType type);
	shared_ptr<Transform> GetTransform();
	shared_ptr<Transform> GetOrAddTransform();
	void AddComponent(shared_ptr<Component> component);
	void AddRenderComponent();

	// Mesh, Material
	void SetMesh(const shared_ptr<Mesh>& mesh);
	void SetMaterial(const shared_ptr<Material>& material);

	// Transform
	bool IsTransformChanged() { return _bTransformChanged; }
	void SetTransformChanged(bool bChanged) { _bTransformChanged = bChanged; }

	// For Debug
	string _actorName{};

protected:

	shared_ptr<RenderComponent> GetRenderComponent() const;
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;

private:
	bool _bRenderable = false;
	bool _bTransformChanged = true;
};

