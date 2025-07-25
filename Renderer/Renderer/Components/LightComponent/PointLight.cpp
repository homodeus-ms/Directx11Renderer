#include "pch.h"
#include "PointLight.h"
#include "Managers/ShaderParameterManager.h"
#include "Components/Transform.h"

PointLight::PointLight()
{
	_desc.ambient = Vec4(0.3f);
	_desc.diffuse = { 1.f, 1.f, 0.2f, 1.f };
	_desc.specular = { 0.7f, 0.7f, 0.3f, 1.f };
	_desc.position = { 0.f, 2.f, 0.f };
}

PointLight::~PointLight()
{
}

void PointLight::BeginPlay()
{
	Super::BeginPlay();

	Vec3 defaultSpotPosition = { -2.f, 6.f, 2.f };
	GetOwnerTransform()->SetWorldPosition(defaultSpotPosition);
}

void PointLight::Tick()
{
	Super::Tick();

	Vec3 pos = GetOwnerTransform()->GetWorldPosition();
	_desc.position = pos;
	
	SHADER_PARAM_MANAGER->PushPointLightData(_desc);
}

void PointLight::SetIsOn(bool bIsOn)
{
	_desc.isOn = bIsOn ? 1 : 0;
	SHADER_PARAM_MANAGER->PushPointLightData(_desc);
}
