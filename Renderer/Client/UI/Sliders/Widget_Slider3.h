#pragma once
#include "Widget_SliderBase.h"

class Widget_Slider3 : public Widget_SliderBase
{
	using Super = Widget_SliderBase;

public:
	Widget_Slider3(const string& addedLabel);
	virtual ~Widget_Slider3();

	virtual void Construct(string sliderName, float* values, bool useSync = false, float min = 0.f, float max = 1.f) override;
	virtual void Tick() override;

	void SetShowFloatDigit(int32 digit);

private:
	string _showFloatDigit = "%.1f";
};

