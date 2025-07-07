#pragma once
#include "Components/Component.h"
#include "Structs/MeshStructDatas.h"

class TransformComponent;
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

	shared_ptr<Component> GetFixedComponent(ComponentType type);
	shared_ptr<TransformComponent> GetTransformComponent();
	shared_ptr<TransformComponent> GetOrAddTransform();
	void AddComponent(shared_ptr<Component> component);

	void SetStaticMeshInfo(const StaticMeshInfo& info);

protected:
	void AddRenderComponent();
	shared_ptr<RenderComponent> GetRenderComponent() const;

	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	
private:

};

