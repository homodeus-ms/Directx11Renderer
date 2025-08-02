#pragma once
#include "Filter.h"
class CombineFilter : public Filter
{
	using Super = Filter;

public:
	CombineFilter();
	virtual ~CombineFilter();

	virtual void Tick() override;
	float* GetFilterStrengthPtr() { return &_filterStrength; }

private:
	float _filterStrength = 1.f;
};

