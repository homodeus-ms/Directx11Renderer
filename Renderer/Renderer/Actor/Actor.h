#pragma once
#include "Components/Component.h"
#include "Graphics/Buffer/ConstantBuffer.h"
#include "Structs/EShaderStage.h"

class Transform;
class BasicMesh;
class RenderComponentBase;
class BasicMeshRenderer;
class StaticMeshRenderer;
class StaticMesh;

enum class EActorType : uint8
{
	Actor,
	Pawn,
	CameraActor,
	LightActor,
};

class Actor : public enable_shared_from_this<Actor>
{
public:
	Actor(EActorType actorType);
	virtual ~Actor();

	virtual void Construct();
	virtual void BeginPlay();
	virtual void Tick();
	virtual void LateTick();
	virtual void FixedTick();

	void Render();

	EActorType GetActorType() const { return _actorType; }

	// Component
	shared_ptr<Component> GetFixedComponent(EComponentType type);
	shared_ptr<Transform> GetTransform();
	shared_ptr<Transform> GetOrAddTransform();
	void AddComponent(shared_ptr<Component> component);

	// Mesh, Material
	void SetBasicMesh(const shared_ptr<BasicMesh>& mesh);
	void SetBasicMaterial(const shared_ptr<Material>& material);
	void SetStaticMesh(const shared_ptr<StaticMesh>& staticMesh);
	shared_ptr<BasicMeshRenderer> GetOrAddBasicMeshRenderer();
	shared_ptr<StaticMeshRenderer> GetOrAddStaticMeshRenderer();

	// Transform
	bool IsTransformChanged() { return _bTransformChanged; }
	virtual void SetTransformChanged(bool bChanged) { _bTransformChanged = bChanged; }


protected:

	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	
	// Renderer Cache
	shared_ptr<RenderComponentBase> _renderer;

	EActorType _actorType;

private:
	
	bool _bTransformChanged = true;
};

