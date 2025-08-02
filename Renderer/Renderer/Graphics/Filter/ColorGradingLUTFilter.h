#pragma once
#include "Filter.h"

class ColorGradingLUTFilter : public Filter
{
	using Super = Filter;

public:
	ColorGradingLUTFilter();
	virtual ~ColorGradingLUTFilter();
	
	void SetNewLUT(const wstring& newLUTName);

private:
	void GetLUT(const wstring& LUTName);

	wstring _LUTName{};
	shared_ptr<LUT> _LUT = nullptr;
	bool _bLUTChanged = false;
};

