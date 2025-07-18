#pragma once
#include "Scene/Scene.h"
#include "Utils/Delegate.h"

#define SCENE_MANAGER GET_SINGLE(SceneManager)
#define SCENE SCENE_MANAGER->GetCurrentScene()

DECLARE_MULTICAST_DELEGATE(BeforeSceneConstructDelegate);
DECLARE_MULTICAST_DELEGATE(BeforeSceneBeginPlayDelegate);

class SceneManager
{
	DECLARE_SINGLE(SceneManager);

public:

	void BeginPlay();
	void Tick();
	void Render();

	template<typename T>
	void ChangeScene(shared_ptr<T> scene)
	{
		_currentScene = scene;
		scene->BeginPlay();
	}

	shared_ptr<Scene> GetCurrentScene() { return _currentScene; }
	void AddActors(const vector<shared_ptr<Actor>>& actors);
	void RemoveActor(const shared_ptr<Actor>& actor);

	// Delegates
	BeforeSceneConstructDelegate _onBeforeSceneConstruct;
	BeforeSceneBeginPlayDelegate _onBeforeSceneBeginPlay;

private:

	shared_ptr<Scene> _currentScene = make_shared<Scene>();
};

