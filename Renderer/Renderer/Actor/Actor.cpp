#include "pch.h"
#include "Actor.h"
#include "Components/RenderComponent.h"
#include "Components/TransformComponent.h"

Actor::Actor()
{
	
}

Actor::~Actor()
{
}

void Actor::Construct()
{
	AddRenderComponent();

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
			component->Tick();
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
	GetRenderComponent()->Render();
}

shared_ptr<Component> Actor::GetFixedComponent(ComponentType type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return _components[index];
}

shared_ptr<TransformComponent> Actor::GetTransformComponent()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Transform);
	return static_pointer_cast<TransformComponent>(component);
}

shared_ptr<TransformComponent> Actor::GetOrAddTransform()
{
	if (GetTransformComponent() == nullptr)
	{
		shared_ptr<TransformComponent> transform = make_shared<TransformComponent>();
		AddComponent(transform);
	}

	return GetTransformComponent();
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
