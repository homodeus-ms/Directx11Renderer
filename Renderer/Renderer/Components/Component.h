#pragma once

class Actor;
class Transform;

enum class ComponentType : uint8
{
	Transform,
	Controller,
	MeshRenderer,
	Camera,
	Animator,

	End,
};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8>(ComponentType::End) - 1
};

class Component
{
public:
	Component(ComponentType type);
	virtual ~Component();

	virtual void Construct() {}
	virtual void BeginPlay() {}
	virtual void Tick() {}
	virtual void LateTick() {}
	virtual void FixedTick() {}

public:
	ComponentType GetType() { return _type; }

	shared_ptr<Actor> GetOwner();
	shared_ptr<Transform> GetOwnerTransform();

private:
	friend class Actor;
	void SetOwner(shared_ptr<Actor> owner) { _owner = owner; }

protected:
	ComponentType _type;
	weak_ptr<Actor> _owner;

};


