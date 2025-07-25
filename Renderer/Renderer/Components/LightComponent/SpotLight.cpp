#include "pch.h"
#include "SpotLight.h"
#include "Managers/ShaderParameterManager.h"
#include "Components/Transform.h"

SpotLight::SpotLight()
{
	_desc.ambient = Vec4(0.4f);
	_desc.diffuse = { 2.f, 2.f, 2.f, 1.f }; 
	_desc.specular = { 2.f, 2.f, 2.f, 1.f };
	_desc.position = { 0.f, 0.f, -5.f };
}

SpotLight::~SpotLight()
{
}

void SpotLight::BeginPlay()
{
	Super::BeginPlay();

	Vec3 defaultSpotPosition = { 0.f, 4.f, -10.f };
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

