#include "pch.h"
#include "SceneManager.h"

void SceneManager::BeginPlay()
{
	_currentScene->Construct();
	_currentScene->BeginPlay();
}

void SceneManager::Tick()
{
	if (_currentScene == nullptr)
		return;

	_currentScene->Tick();
	_currentScene->LateTick();
}

void SceneManager::Render()
{
	_currentScene->Render();
}


void SceneManager::AddActors(const vector<shared_ptr<Actor>>& actors)
{
	if (_currentScene == nullptr)
		return;
	
	for (const shared_ptr<Actor>& actor : actors)
		_currentScene->Add(actor);
}

void SceneManager::RemoveActor(const shared_ptr<Actor>& actor)
{
	if (_currentScene == nullptr)
		return;

	_currentScene->Remove(actor);
}
