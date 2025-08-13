#include "pch.h"
#include "SpotLight.h"
#include "Managers/ShaderParameterManager.h"
#include "Components/Transform.h"

SpotLight::SpotLight()
	: _desc(Color(0.1f, 0.1f, 0.1f, 1.f), Color(2.f, 2.f, 1.2f, 1.f), Color(1.f, 1.f, 1.f, 1.f))
{
	_desc.position = { 0.f, 10.f, -1.5f };
}

SpotLight::~SpotLight()
{
}

void SpotLight::BeginPlay()
{
	Super::BeginPlay();

	//Vec3 defaultSpotPosition = { 0.f, 4.f, -10.f }; 
	Vec3 defaultSpotPosition = { -6.f, 12.f, -6.f };
	GetOwnerTransform()->SetWorldPosition(defaultSpotPosition);
	Vec3 lookTarget = -defaultSpotPosition;
	lookTarget.Normalize();
	_desc.direction = lookTarget;
	GetOwnerTransform()->SetLocalRotationByTargetLook(lookTarget);
}

void SpotLight::Tick()
{
	Super::Tick();

	Vec3 pos = GetOwnerTransform()->GetWorldPosition();
	Vec3 look = GetOwnerTransform()->GetLook();
	_desc.position = pos;
	_desc.direction = look;
	SHADER_PARAM_MANAGER->PushSpotLightData(_desc);
}

void SpotLight::SetIsOn(bool bIsOn)
{
	_desc.isOn = bIsOn ? 1 : 0;
	SHADER_PARAM_MANAGER->PushSpotLightData(_desc);
}

