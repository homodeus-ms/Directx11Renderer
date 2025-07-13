#include "pch.h"
#include "Actor.h"
#include "Components/StaticMeshRenderer.h"
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

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Construct();
	}
}

void Actor::BeginPlay()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->BeginPlay();
	}
}

void Actor::Tick()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
		{
			component->Tick();
		}
	}
}

void Actor::LateTick()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->LateTick();
	}
}

void Actor::FixedTick()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->FixedTick();
	}
}

void Actor::Render()
{
	if (_staticMeshRenderer)
		_staticMeshRenderer->Render();
}


shared_ptr<Component> Actor::GetFixedComponent(EComponentType type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return _components[index];
}

shared_ptr<Transform> Actor::GetTransform()
{
	shared_ptr<Component> component = GetFixedComponent(EComponentType::Transform);
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

		// Renderer Caching
		if (index == static_cast<uint8>(EComponentType::StaticMeshRenderer))
			_staticMeshRenderer = static_pointer_cast<StaticMeshRenderer>(component);
	}
}

void Actor::SetMesh(const shared_ptr<Mesh>& mesh)
{
	// TODO : Default Basic Mesh 처리를 어떻게 할지?
	//GetRenderComponent()->SetMesh(mesh);
}

void Actor::SetStaticMesh(const shared_ptr<StaticMesh>& staticMesh)
{
	GetOrAddStaticMeshRenderer()->SetStaticMesh(staticMesh);
}

void Actor::SetMaterial(const shared_ptr<Material>& material)
{
	//GetRenderComponent()->SetMaterial(material);
}

shared_ptr<StaticMeshRenderer> Actor::GetOrAddStaticMeshRenderer()
{
	if (shared_ptr<Component> component = _components[static_cast<uint8>(EComponentType::StaticMeshRenderer)])
	{
		return static_pointer_cast<StaticMeshRenderer>(component);
	}

	shared_ptr<Component> staticMeshRenderer = make_shared<StaticMeshRenderer>();
	AddComponent(staticMeshRenderer);


	return static_pointer_cast<StaticMeshRenderer>(staticMeshRenderer);
}
