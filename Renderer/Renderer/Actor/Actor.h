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

	virtual void Render();

	// Component
	shared_ptr<Component> GetFixedComponent(ComponentType type);
	shared_ptr<Transform> GetTransform();
	shared_ptr<Transform> GetOrAddTransform();
	void AddComponent(shared_ptr<Component> component);

	// Mesh
	void SetStaticMeshInfo(const StaticMeshInfo& info);

	// Transform
	bool IsTransformChanged() { return _bTransformChanged; }
	void SetTransformChanged(bool bChanged) { _bTransformChanged = bChanged; }

	// Camera
	virtual void AttachFollowCamera(shared_ptr<Actor> cameraActor, bool bKeyInputForCameraMovement = true);
	bool HasCamera() const { return _bHasCamera; }

	// For Debug
	string _actorName{};

protected:

	void AddRenderComponent();

	shared_ptr<RenderComponent> GetRenderComponent() const;
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	
	bool _bHasCamera = false;
	shared_ptr<Actor> _cameraActor = nullptr;

private:
	
	bool _bTransformChanged = true;
};

