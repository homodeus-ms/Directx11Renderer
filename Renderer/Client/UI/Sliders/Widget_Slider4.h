#pragma once
#include "Widget_SliderBase.h"

class Widget_Slider4 : public Widget_SliderBase
{
	using Super = Widget_SliderBase;

public:
	Widget_Slider4(const string& addedLabel);
	virtual ~Widget_Slider4();

	virtual void Construct(string sliderName, float* values, bool useSync = true, float min = 0.f, float max = 1.f) override;
	virtual void Tick() override;
	virtual float* TickSlider() override;

	void UpdateValue(float* val);

private:
	void HandleSync();
	array<float, 4> _preValue;

};

