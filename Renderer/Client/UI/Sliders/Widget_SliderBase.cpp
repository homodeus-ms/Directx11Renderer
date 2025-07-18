#include "pch.h"
#include "Widget_SliderBase.h"

Widget_SliderBase::Widget_SliderBase(const string& addedLabel)
	: _addedLabel(addedLabel)
{
}

Widget_SliderBase::~Widget_SliderBase()
{
}

void Widget_SliderBase::Construct(string sliderName, float* values, bool useSync, float min, float max)
{
	_sliderName = sliderName + "##" + _addedLabel;
	_syncLabel += "##" + _sliderName;
	_values = values;
	_bUseSync = useSync;
	_min = min;
	_max = max;
}
