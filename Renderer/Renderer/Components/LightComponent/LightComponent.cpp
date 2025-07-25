#include "pch.h"
#include "LightComponent.h"

LightComponent::LightComponent()
	: Super(EComponentType::Light)
{
}

LightComponent::~LightComponent()
{
}

void LightComponent::BeginPlay()
{
}

void LightComponent::Tick()
{
	Super::Tick();
}
