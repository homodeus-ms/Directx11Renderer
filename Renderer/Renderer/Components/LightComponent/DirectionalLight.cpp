#include "pch.h"
#include "DirectionalLight.h"
#include "Components/Transform.h"
#include "Actor/Actor.h"
#include "Managers/ShaderParameterManager.h"

DirectionalLight::DirectionalLight()
{
	_desc.ambient = Vec4(0.2f, 0.2f, 0.2f, 1.f);
	_desc.diffuse = Vec4(0.4f, 0.4f, 0.4f, 1.f);
	_desc.specular = Vec4(0.4f, 0.4f, 0.4f, 1.f);
	_desc.direction = Vec3(-1.f, -1.f, 1.f);
	_desc.direction.Normalize();
	_desc.isOn = 1;
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::BeginPlay()
{
	Super::BeginPlay();

	assert(_owner.lock());
}

void DirectionalLight::Tick()
{
	Super::Tick();

	SHADER_PARAM_MANAGER->PushDirectionalLightData(_desc);
}


void DirectionalLight::SetIsOn(bool bIsOn)
{
	_desc.isOn = bIsOn ? 1 : 0;
}

