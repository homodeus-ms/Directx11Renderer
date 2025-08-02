#include "pch.h"
#include "LightManager.h"
#include "Actor/LightActor.h"
#include "Components/LightComponent/LightComponent.h"
#include "Components/LightComponent/DirectionalLight.h"
#include "Components/LightComponent/SpotLight.h"
#include "Components/LightComponent/PointLight.h"
#include "Managers/ShaderParameterManager.h"
#include "Resource/Texture/LoadedTexture.h"
#include "Components/Transform.h"

LightManager::LightManager()
{
	//AddDefaultDirectionalLight();
}

LightManager::~LightManager()
{
}

void LightManager::Construct()
{
	AddDefaultDirectionalLight();

}

shared_ptr<LightActor> LightManager::AddDefaultDirectionalLight()
{
	_globalLightCache = make_shared<LightActor>(ELightType::Directional);
	_globalLightCache->Construct();
	
	{
		Vec3 startPos = { 10.f, 10.f, -10.f };
		_globalLightCache->GetOrAddTransform()->SetWorldPosition(startPos);
		
		LightDesc* desc = _globalLightCache->GetDesc();
		DirectionalLightDesc* direcDesc = static_cast<DirectionalLightDesc*>(desc);
		direcDesc->direction = -startPos;
		direcDesc->direction.Normalize();

		_globalLightCache->GetTransform()->SetLocalRotationByTargetLook(direcDesc->direction);
	}

	return _globalLightCache;
}

shared_ptr<LightActor> LightManager::IncreaseSpotLightOrNull()
{
	if (CanAddSpotLight())
	{
		shared_ptr<LightActor> actor = make_shared<LightActor>(ELightType::Spot);
		IncreseSpotLightCount();
		return actor;
	}
	return nullptr;
}

shared_ptr<LightActor> LightManager::IncreasePointLightOrNull()
{
	if (CanAddPointLight())
	{
		shared_ptr<LightActor> actor = make_shared<LightActor>(ELightType::Point);
		IncresePointLightCount();
		return actor;
	}
	return nullptr;
}

void LightManager::IncreseSpotLightCount()
{
	++_currentSpotLightCount;
}

void LightManager::IncresePointLightCount()
{
	++_currentPointLightCount;
}

void LightManager::TurnDirectionalLightOnOff(bool bTurnOn)
{
	_globalLightCache->SetOnOff(bTurnOn);
}

void LightManager::ReduceLight(shared_ptr<LightActor> actor)
{
	static_pointer_cast<LightActor>(actor)->SetOnOff(false);

	ELightType type = actor->GetLightType();

	if (type == ELightType::Spot)
		--_currentSpotLightCount;
	else if (type == ELightType::Point)
		--_currentPointLightCount;
}

void LightManager::SetEnvLightTexture(const wstring& textureName)
{
	const wstring specName = textureName + L"_spec";
	const wstring diffName = textureName + L"_diff";

	// Specular
	{
		_envTextureSpec = RESOURCE_MANAGER->Get<LoadedTexture>(specName);
		if (_envTextureSpec == nullptr)
		{
			LOG(Log, "Can't find Texture");
			return;
		}

		_envSpecBindingInfo = make_shared<SRVBindingInfo>();
		_envSpecBindingInfo->slot = static_cast<uint8>(ETextureType::IBL_Spec);
		_envSpecBindingInfo->stage = EShaderStage::PsStage;
		_envSpecBindingInfo->srv = _envTextureSpec->GetSRV();
		SHADER_PARAM_MANAGER->PushEnvLight(_envSpecBindingInfo);
	}

	// Diffuse
	{
		_envTextureDiff = RESOURCE_MANAGER->Get<LoadedTexture>(diffName);
		if (_envTextureDiff == nullptr)
		{
			LOG(Log, "Can't find Texture");
			return;
		}

		_envDiffBindingInfo = make_shared<SRVBindingInfo>();
		_envDiffBindingInfo->slot = static_cast<uint8>(ETextureType::IBL_Diff);
		_envDiffBindingInfo->stage = EShaderStage::PsStage;
		_envDiffBindingInfo->srv = _envTextureDiff->GetSRV();
		SHADER_PARAM_MANAGER->PushEnvLight(_envDiffBindingInfo);
	}
}

void LightManager::TurnEnvLightOnOff(bool bOn)
{
	if (_envSpecBindingInfo == nullptr || _envDiffBindingInfo == nullptr)
		return;
	
	SHADER_PARAM_MANAGER->PushEnvLightOnOff(bOn);
}

bool LightManager::CanAddSpotLight()
{
    return _currentSpotLightCount < MAX_SPOT_LIGHT_COUNT;
}

bool LightManager::CanAddPointLight()
{
    return _currentPointLightCount < MAX_POINT_LIGHT_COUNT;;
}

