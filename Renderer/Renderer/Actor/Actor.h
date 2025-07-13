#pragma once
#include "Components/Component.h"
#include "Graphics/Buffer/ConstantBuffer.h"
#include "Structs/EShaderStage.h"

class Transform;
class Mesh;
class RenderComponentBase;
class StaticMeshRenderer;
class StaticMesh;

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
	shared_ptr<Component> GetFixedComponent(EComponentType type);
	shared_ptr<Transform> GetTransform();
	shared_ptr<Transform> GetOrAddTransform();
	void AddComponent(shared_ptr<Component> component);

	// Mesh, Material
	void SetMesh(const shared_ptr<Mesh>& mesh);
	void SetStaticMesh(const shared_ptr<StaticMesh>& staticMesh);
	void SetMaterial(const shared_ptr<Material>& material);
	shared_ptr<StaticMeshRenderer> GetOrAddStaticMeshRenderer();

	// Transform
	bool IsTransformChanged() { return _bTransformChanged; }
	void SetTransformChanged(bool bChanged) { _bTransformChanged = bChanged; }

	// For Debug
	string _actorName{};

protected:

	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	
	// Renderer Cache
	shared_ptr<StaticMeshRenderer> _staticMeshRenderer;

private:
	
	bool _bTransformChanged = true;
};

