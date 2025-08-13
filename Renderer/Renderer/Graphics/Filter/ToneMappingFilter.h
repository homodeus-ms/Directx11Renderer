#pragma once
#include "Filter.h"

class ToneMappingFilter : public Filter
{
	using Super = Filter;

public:
	ToneMappingFilter();
	virtual ~ToneMappingFilter();

	virtual void Tick() override;

	float* GetExposurePtr() { return &_exposure; }
	float* GetGammaPtr() { return &_gamma; }

private:
	float _exposure = 1.f;
	float _gamma = 2.2f;
	
};

