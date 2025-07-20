#pragma once
#include "Structs/LightTypes.h"
#include "Utils/Delegate.h"
#include "Managers/LightManager.h"
#include "Engine/Core/CommandQueue.h"

class Actor;
class CameraActor;
class LightActor;

DECLARE_MULTICAST_DELEGATE(OnLightManagerCreatedDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(OnRenderedActorRegistered, weak_ptr<Actor>);

class Scene : public enable_shared_from_this<Scene>
{
public:
	Scene();
	virtual ~Scene();

	virtual void Construct();
	virtual void BeginPlay();
	virtual void Tick();
	virtual void LateTick();
	virtual void Render();

	virtual void AddActor(shared_ptr<Actor> actor);
	virtual void RemoveActor(shared_ptr<Actor> actor);
	vector<shared_ptr<Actor>> GetRenderedActors();

	// Camera
	shared_ptr<CameraActor> GetMainCamera() const { return _mainCamera; }
	Vec3 GetMainCameraLook() const;
	Matrix GetMainCameraVP();

	// Lights
	shared_ptr<LightActor> GetGlobalLight();
	shared_ptr<LightActor> AddSpotLight();
	shared_ptr<LightActor> AddPointLightOrNull();
	void RemoveLight(shared_ptr<LightActor> actor);
	void TurnGlobalLightOnOff(bool bTurnOn);
	void CreateEnvironment(const wstring& textureName, bool bSetEnvLighting = true);
	void SetEnvLightTexture(const wstring& textureName);
	void TurnEnvLightOnOff(bool bOn);

	OnLightManagerCreatedDelegate _onLightManagerCreated;
	OnRenderedActorRegistered _onRenderedActorRegistered;

	void OnMainCameraLookChangedCallback(const Vec3& look);

private:
	friend class Renderer;

	void RegisterActor(const shared_ptr<Actor>& actor);
	void DeregisterActor(const shared_ptr<Actor>& actor);
	bool IsLightActor(const shared_ptr<Actor>& actor);
	void CheckAndAddLightActor(const shared_ptr<Actor>& actor);
	void AddLightActor(shared_ptr<Actor> actor);
	
	CommandQueue* _commands;
	LightManager* _lightManager;
	unordered_set<shared_ptr<Actor>> _actors;
	shared_ptr<CameraActor> _mainCamera;
	
	shared_ptr<Actor> _cubeMapCached = nullptr;
};

