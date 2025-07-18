#include "pch.h"
#include "PointLight.h"
#include "Managers/ShaderParameterManager.h"

PointLight::PointLight()
{
	_desc.ambient = Vec4(0.4f);
	_desc.diffuse = { 2.f, 2.f, 1.f, 1.f };
	_desc.specular = { 2.f, 2.f, 1.f, 1.f };
	_desc.position = { 0.f, 2.f, 0.f };
}

PointLight::~PointLight()
{
}

void PointLight::Tick()
{
	Super::Tick();
	
	SHADER_PARAM_MANAGER->PushPointLightData(_desc);
}

void PointLight::SetIsOn(bool bIsOn)
{
	_desc.isOn = bIsOn ? 1 : 0;
}
