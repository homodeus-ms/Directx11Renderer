#include "pch.h"
#include "Actor.h"
#include "Components/BasicMeshRenderer.h"
#include "Components/StaticMeshRenderer.h"
#include "Components/Transform.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent/LightComponent.h"
#include "LightActor.h"

Actor::Actor(EActorType actorType, const string& name)
	: _actorType(actorType)
{
	_actorName = RENDERER->GetNameManager()->AddOrGetNewNameIfDuplicated(name);

	if (_actorType == EActorType::CameraActor)
	{
		_bIsRenderedActor = false;
	}

	if (_actorType == EActorType::LightActor || _actorType == EActorType::CameraActor
		|| _actorType == EActorType::DebugActor)
	{
		_bCastShadow = false;
	}
}

Actor::~Actor()
{
	RENDERER->GetNameManager()->RemoveName(_actorName);
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
			if (component->GetType() == EComponentType::Light)
			{
				shared_ptr<LightActor> lA = static_pointer_cast<LightActor>(component->GetOwner());
				if (!lA || lA->GetLightType() != ELightType::Directional)
				{
					int a = 3;
				}
			}
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
	if (_renderer)
		_renderer->Render();
}

void Actor::RenderDepthOnly(bool bForPointLight)
{
	if (_bCastShadow && _renderer)
		_renderer->RenderDepthOnly(bForPointLight);
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
		if (index == static_cast<uint8>(EComponentType::StaticMeshRenderer) ||
			index == static_cast<uint8>(EComponentType::BasicMeshRenderer))
		{
			_renderer = static_pointer_cast<RenderComponentBase>(component);
		}
	}
}

bool Actor::IsRenderedActor()
{
	return _bIsRenderedActor;
}

vector<shared_ptr<Material>> Actor::GetMaterials()
{
	if (!IsRenderedActor())
		return vector<shared_ptr<Material>>();

	return _renderer->GetMaterials();
}

void Actor::SetBasicMesh(const shared_ptr<BasicMesh>& mesh)
{
	// TODO : Default Basic Mesh 처리를 어떻게 할지?
	GetOrAddBasicMeshRenderer()->SetBasicMesh(mesh);
}

void Actor::SetBasicMaterial(const shared_ptr<Material>& material)
{
	GetOrAddBasicMeshRenderer()->SetBasicMaterial(material);
}

void Actor::SetStaticMesh(const shared_ptr<StaticMesh>& staticMesh)
{
	GetOrAddStaticMeshRenderer()->SetStaticMesh(staticMesh);
}

void Actor::ChangeMaterialType(EMaterialType type)
{
	if (!IsRenderedActor())
		return;

	return _renderer->ChangeMaterialType(type);
}

shared_ptr<BasicMeshRenderer> Actor::GetOrAddBasicMeshRenderer()
{
	if (shared_ptr<Component> component = _components[static_cast<uint8>(EComponentType::BasicMeshRenderer)])
	{
		return static_pointer_cast<BasicMeshRenderer>(component);
	}

	shared_ptr<Component> basicMeshRenderer = make_shared<BasicMeshRenderer>();
	AddComponent(basicMeshRenderer);

	return static_pointer_cast<BasicMeshRenderer>(basicMeshRenderer);
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
