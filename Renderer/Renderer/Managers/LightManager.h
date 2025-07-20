#pragma once
#include "Structs/LightTypes.h"

class LightActor;
class Texture;

class LightManager
{
public:
	LightManager();
	virtual ~LightManager();

	void Construct();

	shared_ptr<LightActor> GetGlobalLight() { return _globalLightCache; }

	shared_ptr<LightActor> AddDefaultDirectionalLight();
	shared_ptr<LightActor> IncreaseSpotLightOrNull();
	shared_ptr<LightActor> IncreasePointLightOrNull();
	void IncreseSpotLightCount();
	void IncresePointLightCount();

	void TurnDirectionalLightOnOff(bool bTurnOn);
	void ReduceLight(shared_ptr<LightActor> actor);
	void SetEnvLightTexture(const wstring& textureName);
	void TurnEnvLightOnOff(bool bOn);

private:
	
	bool CanAddSpotLight();
	bool CanAddPointLight();
	
	uint8 _currentSpotLightCount = 0;
	uint8 _currentPointLightCount = 0;

	shared_ptr<LightActor> _globalLightCache;
	
	shared_ptr<Texture> _envTexture;
	shared_ptr<struct SRVBindingInfo> _envBindingInfo;
};

