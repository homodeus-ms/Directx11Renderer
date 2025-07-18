#include "pch.h"
#include "Widget_Slider3.h"
#include <format>

Widget_Slider3::Widget_Slider3(const string& addedLabel)
	: Super(addedLabel)
{
}

Widget_Slider3::~Widget_Slider3()
{
}

void Widget_Slider3::Construct(string sliderName, float* values, bool useSync, float min, float max)
{
	Super::Construct(sliderName, values, useSync, min, max);

}

void Widget_Slider3::Tick()
{
	ImGui::SliderFloat3(_sliderName.c_str(), _values, _min, _max, _showFloatDigit.c_str());
}

void Widget_Slider3::SetShowFloatDigit(int32 digit)
{
	digit = ::clamp(digit, 0, 3);

	_showFloatDigit = std::format("%.{0}f", digit);;
}


