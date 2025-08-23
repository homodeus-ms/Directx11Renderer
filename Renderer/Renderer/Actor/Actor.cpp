#include "pch.h"
#include "Actor.h"
#include "Components/BasicMeshRenderer.h"
#include "Components/StaticMeshRenderer.h"
#include "Components/Transform.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent/LightComponent.h"
#include "Components/ReflectComponent.h"
#include "LightActor.h"
#include "Resource/BasicMesh/BasicMesh.h"
#include "Resource/StaticMesh.h"
#include "Resource/SkeletalMesh.h"


Actor::Actor(EActorType actorType, const string& name)
	: _actorType(actorType)
{
	_actorName = RENDERER->GetNameManager()->AddOrGetNewNameIfDuplicated(name);

	if (_actorType == EActorType::CameraActor)
	{
		_bIsRenderedActor = false;
		_bCastShadow = false;
	}
	else if(_actorType == EActorType::LightActor || _actorType == EActorType::DebugActor)
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

	if (_actorType == EActorType::ReflectActor)
	{
		AddComponent(make_shared<ReflectComponent>());
	}

	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Construct();
	}

	for (shared_ptr<Component>& addedComponent : _addedComponent)
		addedComponent->Construct();
}

void Actor::BeginPlay()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->BeginPlay();
	}

	for (shared_ptr<Component>& addedComponent : _addedComponent)
		addedComponent->BeginPlay();
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

	for (shared_ptr<Component>& addedComponent : _addedComponent)
		addedComponent->Tick();
}

void Actor::LateTick()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->LateTick();
	}

	for (shared_ptr<Component>& addedComponent : _addedComponent)
		addedComponent->LateTick();
}

void Actor::FixedTick()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->FixedTick();
	}

	for (shared_ptr<Component>& addedComponent : _addedComponent)
		addedComponent->FixedTick();
}

void Actor::Render()
{
	if (_renderer)
		_renderer->Render();
}

void Actor::RenderDepthMap()
{
	if (_renderer)
		_renderer->RenderDepthOnly(false, 0);
}

void Actor::RenderShadowMap(bool bForPointLight, int32 instanceCount)
{
	if (_bCastShadow && _renderer)
		_renderer->RenderDepthOnly(bForPointLight, instanceCount);
}

void Actor::RenderDrawNormal()
{
	_renderer->RenderDrawNormal();
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

shared_ptr<Component> Actor::GetReflectComponentOrNull()
{
	for (shared_ptr<Component>& c : _addedComponent)
	{
		if (c->GetType() == EComponentType::ReflectComponent)
			return c;
	}
	return nullptr;
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
	else
	{
		if (HasNoSameComponentType(component))
			_addedComponent.push_back(component);
	}
}

bool Actor::IsRenderedActor()
{
	return _bIsRenderedActor;
}

vector<shared_ptr<MaterialBase>> Actor::GetMaterials()
{
	if (!IsRenderedActor())
		return vector<shared_ptr<MaterialBase>>();

	return _renderer->GetMaterials();
}

void Actor::SetBasicMesh(const shared_ptr<BasicMesh>& mesh)
{
	// TODO : Default Basic Mesh 처리를 어떻게 할지?
	_meshType = mesh->GetType();
	_mesh = mesh;
	GetOrAddBasicMeshRenderer()->SetBasicMesh(mesh);
}

void Actor::SetBasicMaterial(const shared_ptr<MaterialBase>& material)
{
	GetOrAddBasicMeshRenderer()->SetBasicMaterial(material);
}

void Actor::SetStaticMesh(const shared_ptr<StaticMesh>& staticMesh)
{
	_meshType = staticMesh->GetType();
	_mesh = staticMesh;
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

void Actor::SetTransformChanged(bool bChanged)
{
	_bTransformChanged = bChanged; 
	_onTransformChanged.Broadcast();
}

bool Actor::HasNoSameComponentType(shared_ptr<Component> component)
{
	EComponentType newComponentType = component->GetType();
	for (shared_ptr<Component>& c : _addedComponent)
	{
		if (c->GetType() == newComponentType)
			return false;
	}

	return true;
}

