#pragma once
#include "Pawn.h"
#include "Structs/LightTypes.h"

class LightComponent;

class LightActor : public Pawn
{
	using Super = Pawn;

public:
	LightActor(ELightType lightType, const string& name = "LightActor");
	virtual ~LightActor();

	ELightType GetLightType() const { return _lightType; }
	LightDesc* GetDesc() const;

	virtual void BeginPlay() override;
	virtual void Tick() override;

	bool IsOn() const { return _bIsOn; }
	void SetOnOff(bool bIsOn);

protected:

	ELightType _lightType;
	shared_ptr<LightComponent> _lightComponent;
	bool _bIsOn = true;
};

