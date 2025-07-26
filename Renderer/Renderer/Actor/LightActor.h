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
	float GetRange();

	virtual void Construct() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;

	bool IsOn() const { return _bIsOn; }
	void SetOnOff(bool bIsOn);

	// VP for shadowMap
	Matrix GetLightMatV() { return _matV; }
	Matrix GetLightMatP() { return _matP; }
	Matrix GetLightVP() { UpdateVP();  _VP = _matV * _matP; return _VP; }
	array<Matrix, 6> GetLightVPForPointLight();
	bool IsThisPointLightUseShadowMap();
	void GetLightVPForPointLight(OUT vector<Matrix>& views, OUT vector<Matrix>& VPs);
	void UpdateVP();
	shared_ptr<SRVBindingInfo> GetShadowSRVInfo() { return _shadowSRVInfo; }
	void SetShadowSRVInfo(shared_ptr<SRVBindingInfo> info) { _shadowSRVInfo = info; }
	void SetShadowMapIndex(int32 index);

protected:
	
	ELightType _lightType;
	shared_ptr<LightComponent> _lightComponent;
	bool _bIsOn = true;
	shared_ptr<SRVBindingInfo> _shadowSRVInfo;

	Matrix _matV = Matrix::Identity;
	Matrix _matP = Matrix::Identity;
	Matrix _VP = Matrix::Identity;
};

