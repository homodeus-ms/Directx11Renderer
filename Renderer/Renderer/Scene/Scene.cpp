#include "pch.h"
#include "Scene.h"
#include "Actor/Actor.h"
#include "Actor/CameraActor.h"
#include "Components/Transform.h"
#include "Managers/RenderManager.h"

// TEMP
#include "Managers/ShaderParameterManager.h"

void Scene::Construct()
{
	_mainCamera = make_shared<CameraActor>();
	_mainCamera->Construct();
}

void Scene::BeginPlay()
{
	unordered_set<shared_ptr<Actor>> actors = _actors;

	for (shared_ptr<Actor> actor : actors)
	{
		actor->BeginPlay();
	}

	_mainCamera->BeginPlay();
}

void Scene::Tick()
{
	unordered_set<shared_ptr<Actor>> actors = _actors;

	for (shared_ptr<Actor> actor : actors)
	{
		actor->Tick();
	}

	_mainCamera->Tick();

	// TEMP
	SHADER_PARAM_MANAGER->Update();

	// 여기서 actor들 Render
	for (shared_ptr<Actor> actor : actors)
	{
		actor->Render();
	}
}

void Scene::LateTick()
{
	unordered_set<shared_ptr<Actor>> actors = _actors;

	for (shared_ptr<Actor> actor : actors)
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

void Scene::Add(shared_ptr<Actor> actor)
{
	_actors.insert(actor);
}

void Scene::Remove(shared_ptr<Actor> actor)
{
	_actors.erase(actor);
}
