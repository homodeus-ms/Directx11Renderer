#pragma once

class Actor;
class CameraActor;

class Scene
{
public:
	virtual void Construct();
	virtual void BeginPlay();
	virtual void Tick();
	virtual void LateTick();
	virtual void Render();

	virtual void Add(shared_ptr<Actor> actor);
	virtual void Remove(shared_ptr<Actor> actor);

	shared_ptr<CameraActor> GetMainCamera() const { return _mainCamera; }

private:
	unordered_set<shared_ptr<Actor>> _actors;

	shared_ptr<CameraActor> _mainCamera;

	//unordered_set<shared_ptr<Actor>> _cameras;
	
	unordered_set<shared_ptr<Actor>> _lights;
};

