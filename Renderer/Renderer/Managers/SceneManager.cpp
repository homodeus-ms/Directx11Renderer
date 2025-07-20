#include "pch.h"
#include "SceneManager.h"
#include "Managers/ShaderParameterManager.h"

void SceneManager::BeginPlay()
{
	_onBeforeSceneConstruct.Broadcast();
	_currentScene->Construct();
	_onBeforeSceneBeginPlay.Broadcast();
	_currentScene->BeginPlay();
	_onAfterSceneBeginPlay.Broadcast();
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
		_currentScene->AddActor(actor);
}

void SceneManager::RemoveActor(const shared_ptr<Actor>& actor)
{
	if (_currentScene == nullptr)
		return;

	_currentScene->RemoveActor(actor);
}
