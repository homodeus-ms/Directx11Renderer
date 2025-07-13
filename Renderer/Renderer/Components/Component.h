#pragma once

class Actor;
class Transform;

enum class EComponentType : uint8
{
	Transform,
	Controller,
	StaticMeshRenderer,
	Camera,
	Animator,
	Light,

	End,
};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8>(EComponentType::End) - 1
};

class Component
{
public:
	Component(EComponentType type);
	virtual ~Component();

	virtual void Construct() {}
	virtual void BeginPlay() {}
	virtual void Tick() {}
	virtual void LateTick() {}
	virtual void FixedTick() {}

public:
	EComponentType GetType() { return _type; }

	shared_ptr<Actor> GetOwner();
	shared_ptr<Transform> GetOwnerTransform();

private:
	friend class Actor;
	void SetOwner(shared_ptr<Actor> owner) { _owner = owner; }

protected:
	EComponentType _type;
	weak_ptr<Actor> _owner;

};


