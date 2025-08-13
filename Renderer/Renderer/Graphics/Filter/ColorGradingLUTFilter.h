#pragma once
#include "Filter.h"

class ColorGradingLUTFilter : public Filter
{
	using Super = Filter;

public:
	ColorGradingLUTFilter();
	virtual ~ColorGradingLUTFilter();
	
	virtual void Tick() override;

	void SetNewLUT(const wstring& newLUTName);
	float* GetLUTMixRatioPtr() { return &_LutMixRatio; }

private:
	void GetLUT(const wstring& LUTName);

	wstring _LUTName{};
	shared_ptr<LUT> _LUT = nullptr;
	bool _bLUTChanged = false;
	float _LutMixRatio = 0.3f;
};

