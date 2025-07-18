#pragma once
#include "Structs/LightTypes.h"

class LightActor;

class LightManager
{
public:
	LightManager();
	virtual ~LightManager();

	shared_ptr<LightActor> GetGlobalLight() { return _globalLightCache.lock(); }

	shared_ptr<LightActor> AddDefaultDirectionalLight();
	shared_ptr<LightActor> AddSpotLight();
	shared_ptr<LightActor> AddPointLight();

	void TurnDirectionalLightOnOff(bool bTurnOn);
	void RemoveLight(shared_ptr<LightActor> actor);

private:
	bool CanAddSpotLight();
	bool CanAddPointLight();
	
	
	uint8 _currentSpotLightCount = 0;
	uint8 _currentPointLightCount = 0;

	weak_ptr<LightActor> _globalLightCache;

	
};

