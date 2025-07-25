#pragma once
#include "Structs/LightTypes.h"
#include "Utils/Delegate.h"
#include "Managers/LightManager.h"
#include "Engine/Core/CommandQueue.h"
#include "Structs/ECameraType.h"

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
	shared_ptr<CameraActor> GetCurrCamera() const { return _currCamera; }
	Vec3 GetCurrCameraLook() const;
	Matrix GetCurrCameraV();
	Matrix GetCurrCameraP();
	Matrix GetCurrCameraVP();
	shared_ptr<CameraActor> SwitchCameraAndGet(ECameraType type);

	// Lights
	shared_ptr<LightActor> GetGlobalLight();
	shared_ptr<LightActor> AddSpotLight();
	shared_ptr<LightActor> AddPointLightOrNull();
	void RemoveLight(shared_ptr<LightActor> actor);
	void TurnGlobalLightOnOff(bool bTurnOn);
	void CreateEnvironment(const wstring& textureName, bool bSetEnvLighting = true);
	void SetEnvLightTexture(const wstring& textureName);
	void TurnEnvLightOnOff(bool bOn);

	// Shadow Map
	bool ShouldDrawDebugShadowMap() { return _bDrawDebugShadowMap; }
	void SetDrawShadowMap(bool bDraw) { _bDrawDebugShadowMap = bDraw; }

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
	void CreateShadowMapDebugActor();
	
	CommandQueue* _commands;
	LightManager* _lightManager;
	unordered_set<shared_ptr<Actor>> _actors;
	vector<shared_ptr<Actor>> _renderedActors;
	shared_ptr<CameraActor> _currCamera;
	shared_ptr<CameraActor> _mainCamera;
	shared_ptr<CameraActor> _topViewCamera;
	
	shared_ptr<Actor> _cubeMapCached = nullptr;
	shared_ptr<Actor> _shadowMapDebugActor = nullptr;
	bool _bDrawDebugShadowMap = false;
};

