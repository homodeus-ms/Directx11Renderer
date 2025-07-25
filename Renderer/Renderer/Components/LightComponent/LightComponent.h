#pragma once
#include "Components/Component.h"
#include "Structs/LightTypes.h"

class LightComponent : public Component
{
	using Super = Component;

public:
	LightComponent();
	virtual ~LightComponent();

	virtual void BeginPlay() override;
	virtual void Tick() override;

	virtual LightDesc* GetDesc() abstract;

	virtual Color GetAmbient() { return GetDesc()->ambient; }
	virtual Color GetDiffuse() { return GetDesc()->diffuse; }
	virtual Color GetSpecular() { return GetDesc()->specular; }
	virtual float GetRange() { return 100.f; }

	virtual void SetAmbient(Color ambient) abstract;
	virtual void SetDiffuse(Color diffuse) abstract;
	virtual void SetSpecular(Color specular) abstract;

	

	virtual void SetIsOn(bool bIsOn) abstract;
};

