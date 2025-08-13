#include "pch.h"
#include "PointLight.h"
#include "Managers/ShaderParameterManager.h"
#include "Components/Transform.h"

bool PointLight::bDrawShadowPointLightSelected = false;

PointLight::PointLight()
	: _desc(Color(0.3f, 0.3f, 0.3f, 1.f), Color(1.f, 1.f, 0.2f, 1.f), Color(0.7f, 0.7f, 0.3f, 1.f))
{
	_desc.position = { 0.f, 8.f, -2.f };
	
	if (!bDrawShadowPointLightSelected)
	{
		bDrawShadowPointLightSelected = true;
		_desc.bShadowMapUsing = 1;
	}
}

PointLight::~PointLight()
{
	if (_desc.bShadowMapUsing == 1)
		bDrawShadowPointLightSelected = false;
}

void PointLight::BeginPlay()
{
	Super::BeginPlay();

	Vec3 defaultPosition = { -2.f, 10.f, -4.f };
	GetOwnerTransform()->SetWorldPosition(defaultPosition);
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
