#include "pch.h"
#include "LightManager.h"
#include "Actor/LightActor.h"
#include "Components/LightComponent/LightComponent.h"
#include "Components/LightComponent/DirectionalLight.h"
#include "Components/LightComponent/SpotLight.h"
#include "Components/LightComponent/PointLight.h"

LightManager::LightManager()
{
	AddDefaultDirectionalLight();
}

LightManager::~LightManager()
{
}

shared_ptr<LightActor> LightManager::AddDefaultDirectionalLight()
{
	shared_ptr<LightActor> actor = make_shared<LightActor>(ELightType::Directional);
	_globalLightCache = actor;
	return actor;
}

shared_ptr<LightActor> LightManager::AddSpotLight()
{
	if (CanAddSpotLight())
	{
		shared_ptr<LightActor> actor = make_shared<LightActor>(ELightType::Spot);
		++_currentSpotLightCount;
		return actor;
	}
	return nullptr;
}

shared_ptr<LightActor> LightManager::AddPointLight()
{
	if (CanAddPointLight())
	{
		shared_ptr<LightActor> actor = make_shared<LightActor>(ELightType::Point);
		++_currentPointLightCount;
		return actor;
	}
	return nullptr;
}

void LightManager::TurnDirectionalLightOnOff(bool bTurnOn)
{
	_globalLightCache.lock()->SetOnOff(bTurnOn);
}

void LightManager::RemoveLight(shared_ptr<LightActor> actor)
{
	ELightType type = actor->GetLightType();

	if (type == ELightType::Spot)
		--_currentSpotLightCount;
	else if (type == ELightType::Point)
		--_currentPointLightCount;
}

bool LightManager::CanAddSpotLight()
{
    return _currentSpotLightCount < MAX_SPOT_LIGHT_COUNT;
}

bool LightManager::CanAddPointLight()
{
    return _currentPointLightCount < MAX_POINT_LIGHT_COUNT;;
}

