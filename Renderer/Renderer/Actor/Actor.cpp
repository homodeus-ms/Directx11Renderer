#include "pch.h"
#include "Actor.h"
#include "Components/RenderComponent.h"
#include "Components/Transform.h"
#include "Components/CameraComponent.h"

Actor::Actor()
	: _actorName("BaseActor")
{
	
}

Actor::~Actor()
{
}

void Actor::Construct()
{
	GetOrAddTransform();
	AddRenderComponent();

	if (HasCamera())
		_cameraActor->Construct();

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Construct();
	}
}

void Actor::BeginPlay()
{
	if (HasCamera())
		_cameraActor->BeginPlay();

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->BeginPlay();
	}
}

void Actor::Tick()
{
	if (HasCamera())
		_cameraActor->Tick();

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Tick();
	}
}

void Actor::LateTick()
{
	if (HasCamera())
		_cameraActor->LateTick();

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->LateTick();
	}
}

void Actor::FixedTick()
{
	if (HasCamera())
		_cameraActor->FixedTick();

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->FixedTick();
	}
}

void Actor::Render()
{
	// TEMP
	if (HasCamera())
	{
		GetRenderComponent()->Render();
	}
}

shared_ptr<Component> Actor::GetFixedComponent(ComponentType type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return _components[index];
}

shared_ptr<Transform> Actor::GetTransform()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Transform);
	return static_pointer_cast<Transform>(component);
}

shared_ptr<Transform> Actor::GetOrAddTransform()
{
	if (GetTransform() == nullptr)
	{
		shared_ptr<Transform> transform = make_shared<Transform>();
		AddComponent(transform);
	}

	return GetTransform();
}

void Actor::AddComponent(shared_ptr<Component> component)
{
	component->SetOwner(shared_from_this());

	uint8 index = static_cast<uint8>(component->GetType());

	if (index < FIXED_COMPONENT_COUNT)
	{
		_components[index] = component;
	}
}

void Actor::SetStaticMeshInfo(const StaticMeshInfo& info)
{
	GetRenderComponent()->SetStaticMeshInfo(info);
}


void Actor::AttachFollowCamera(shared_ptr<Actor> cameraActor, bool bKeyInputForCameraMovement)
{
	_cameraActor = cameraActor;
	_bHasCamera = true;
}


void Actor::AddRenderComponent()
{
	AddComponent(make_shared<RenderComponent>());
}

shared_ptr<RenderComponent> Actor::GetRenderComponent() const
{
	if (shared_ptr<Component> component = _components[static_cast<uint8>(ComponentType::MeshRenderer)])
	{
		return static_pointer_cast<RenderComponent>(component);
	}

	LOG(Warning, "Render Component NOT Exist");
	
	assert(false);

	return nullptr;
}

