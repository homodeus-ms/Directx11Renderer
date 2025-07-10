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
			if (component->GetType() != ComponentType::MeshRenderer)
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
	if (_bRenderable)
		GetRenderComponent()->Render();
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

void Actor::AddRenderComponent()
{
	AddComponent(make_shared<RenderComponent>());
	_bRenderable = true;
}

void Actor::SetMesh(const shared_ptr<Mesh>& mesh)
{
	GetRenderComponent()->SetMesh(mesh);
}

void Actor::SetMaterial(const shared_ptr<Material>& material)
{
	GetRenderComponent()->SetMaterial(material);
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

