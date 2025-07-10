#pragma once
#include "Scene/Scene.h"

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

private:

	shared_ptr<Scene> _currentScene = make_shared<Scene>();
};

