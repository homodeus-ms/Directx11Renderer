#include "pch.h"
#include "SpotLight.h"
#include "Managers/ShaderParameterManager.h"

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

void SpotLight::Tick()
{
	Super::Tick();
	
	SHADER_PARAM_MANAGER->PushSpotLightData(_desc);
}

void SpotLight::SetIsOn(bool bIsOn)
{
	_desc.isOn = bIsOn ? 1 : 0;
	SHADER_PARAM_MANAGER->PushSpotLightData(_desc);
}

