#pragma once
#include "Filter.h"

class BloomPreFilter : public Filter
{
	using Super = Filter;

public:
	BloomPreFilter();
	virtual ~BloomPreFilter();

	virtual void Tick() override;
	float* GetBrightnessPtr() { return &_brightnessThreshold; }


private:
	float _brightnessThreshold = 0.2f;
};

