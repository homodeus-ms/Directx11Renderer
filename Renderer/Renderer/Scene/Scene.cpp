#include "pch.h"
#include "Scene.h"
#include "Managers/SceneManager.h"
#include "Actor/Actor.h"
#include "Actor/CameraActor.h"
#include "Actor/LightActor.h"
#include "Components/Transform.h"
#include "Components/LightComponent/LightComponent.h"
#include "Components/LightComponent/SpotLight.h"
#include "Components/LightComponent/PointLight.h"
#include "Managers/RenderManager.h"
#include "Managers/LightManager.h"


Scene::Scene()
{
	_lightManager = new LightManager();
	_commands = new CommandQueue();
}

Scene::~Scene()
{
	SAFE_DELETE(_lightManager);
	SAFE_DELETE(_commands);
}

void Scene::Construct()
{
	RegisterActor(_lightManager->AddDefaultDirectionalLight());
	_onLightManagerCreated.Broadcast();

	unordered_set<shared_ptr<Actor>> actors = _actors;
	for (const shared_ptr<Actor>& actor : actors)
	{
		actor->Construct();
	}

	_mainCamera = make_shared<CameraActor>();
	_mainCamera->Construct();

	//_mainCamera->_onCameraLookChanged.BindObject(shared_from_this(), &Scene::OnMainCameraLookChangedCallback);
	//_lightManager->SetGlobalLightLook(_mainCamera->GetTransform()->GetLook());
}

void Scene::BeginPlay()
{
	// Actors
	for (const shared_ptr<Actor>& actor : _actors)
	{
		actor->BeginPlay();
	}

	// Camera
	_mainCamera->BeginPlay();
}

void Scene::Tick()
{
	_commands->TickCommands();
	
	for (const shared_ptr<Actor>& actor : _actors)
	{
		actor->Tick();
	}

	_mainCamera->Tick();

	// 여기서 actor들 Render
	for (shared_ptr<Actor> actor : _actors)
	{
		actor->Render();
	}
}

void Scene::LateTick()
{
	for (const shared_ptr<Actor>& actor : _actors)
	{
		actor->LateTick();
	}
}

void Scene::Render()
{
	vector<shared_ptr<Actor>> temp;
	temp.insert(temp.end(), _actors.begin(), _actors.end());
	GET_SINGLE(RenderManager)->Render(temp);
}

void Scene::AddActor(shared_ptr<Actor> actor)
{
	_commands->PushCommand([this, actor]()
		{
			auto self = shared_from_this();

			if (actor->GetActorType() == EActorType::LightActor)
			{
				LOG(Log, "Use AddSpotLight(), AddPointLight()");
				return;
			}

			RegisterActor(actor);
			
			actor->Construct();
			actor->BeginPlay();
		});

	/*if (actor->GetActorType() == EActorType::LightActor)
	{
		CheckAndAddLightActor(actor);
		return;
	}

	_actors.insert(actor);*/
}

void Scene::RemoveActor(shared_ptr<Actor> actor)
{
	_commands->PushCommand([this, actor]()
		{
			auto self = shared_from_this();

			if (IsLightActor(actor))
			{
				_lightManager->RemoveLight(static_pointer_cast<LightActor>(actor));
			}
			
			DeregisterActor(actor);
		});

	/*if (actor->GetActorType() == EActorType::LightActor)
	{
		shared_ptr<LightActor> lightActor = static_pointer_cast<LightActor>(actor);
		_lightManager->RemoveLight(lightActor);
		return;
	}

	_actors.erase(actor);*/
}

shared_ptr<LightActor> Scene::GetGlobalLight()
{
	return _lightManager->GetGlobalLight();
}

Vec3 Scene::GetMainCameraLook() const
{
	return _mainCamera->GetTransform()->GetLook();
}

void Scene::TurnGlobalLightOnOff(bool bTurnOn)
{
	_lightManager->TurnDirectionalLightOnOff(bTurnOn);
}

shared_ptr<LightActor> Scene::AddSpotLight()
{
	const shared_ptr<LightActor>& added = _lightManager->AddSpotLight();
	if (added != nullptr)
		AddLightActor(added);

	return added;
}

shared_ptr<LightActor> Scene::AddPointLight()
{
	const shared_ptr<LightActor>& added = _lightManager->AddPointLight();
	if (added != nullptr)
		AddLightActor(added);

	return added;
}

void Scene::RemoveLight(shared_ptr<LightActor> actor)
{
	RemoveActor(actor);
}

void Scene::OnMainCameraLookChangedCallback(const Vec3& look)
{
	//_lightManager->SetGlobalLightLook(look);
}

void Scene::RegisterActor(const shared_ptr<Actor>& actor)
{
	_actors.insert(actor);
}

void Scene::DeregisterActor(const shared_ptr<Actor>& actor)
{
	_actors.erase(actor);
}

bool Scene::IsLightActor(const shared_ptr<Actor>& actor)
{
	return actor->GetActorType() == EActorType::LightActor;
}

void Scene::AddLightActor(shared_ptr<Actor> actor)
{
	_commands->PushCommand([this, actor]()
		{
			auto self = shared_from_this();
			RegisterActor(actor);
			actor->Construct();
			actor->BeginPlay();
		});
}
