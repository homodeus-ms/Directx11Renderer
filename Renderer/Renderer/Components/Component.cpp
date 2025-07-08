#include "pch.h"
#include "Component.h"
#include "Actor/Actor.h"

Component::Component(ComponentType type)
	: _type(type)
{
}

Component::~Component()
{
}

shared_ptr<Actor> Component::GetOwner()
{
	return _owner.lock();
}

shared_ptr<Transform> Component::GetOwnerTransform()
{
	return _owner.lock()->GetTransform();
}
