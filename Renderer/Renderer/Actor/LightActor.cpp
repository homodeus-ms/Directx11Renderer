#include "pch.h"
#include "LightActor.h"
#include "Components/LightComponent/LightComponent.h"
#include "Components/LightComponent/DirectionalLight.h"
#include "Components/LightComponent/SpotLight.h"
#include "Components/LightComponent/PointLight.h"

LightActor::LightActor(ELightType lightType)
	: Super(EActorType::LightActor), _lightType(lightType)
{
	switch (_lightType)
	{
	case ELightType::Directional:
		_lightComponent = make_shared<DirectionalLight>();
		break;
	case ELightType::Spot:
		_lightComponent = make_shared<SpotLight>();
		break;
	case ELightType::Point:
		_lightComponent = make_shared<PointLight>();
		break;
	default:
		assert(false);
		break;
	}
}

LightActor::~LightActor()
{
}

LightDesc* LightActor::GetDesc() const
{
	return _lightComponent->GetDesc();
}

void LightActor::BeginPlay()
{
	Super::BeginPlay();

	assert(_lightComponent != nullptr);

	AddComponent(_lightComponent);
}

void LightActor::Tick()
{
	Super::Tick();
}

void LightActor::SetOnOff(bool bIsOn)
{
	_bIsOn = bIsOn;
	
	_lightComponent->SetIsOn(bIsOn);

}
