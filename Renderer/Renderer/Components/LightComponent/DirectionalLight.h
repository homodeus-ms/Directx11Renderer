#pragma once
#include "LightComponent.h"

class DirectionalLight : public LightComponent
{
	using Super = LightComponent;

public:
	DirectionalLight();
	virtual ~DirectionalLight();

	virtual void BeginPlay() override;
	virtual void Tick() override;

	virtual LightDesc* GetDesc() override { return &_desc; }
	virtual void SetIsOn(bool bIsOn) override;
	virtual void SetAmbient(Color ambient) override { _desc.ambient = ambient; }
	virtual void SetDiffuse(Color diffuse) override { _desc.diffuse = diffuse; }
	virtual void SetSpecular(Color specular) override { _desc.specular = specular; }

private:
	DirectionalLightDesc _desc;
};

