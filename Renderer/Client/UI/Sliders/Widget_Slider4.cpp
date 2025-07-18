#include "pch.h"
#include "Widget_Slider4.h"
#include <smmintrin.h> 

Widget_Slider4::Widget_Slider4(const string& addedLabel)
	: Super(addedLabel)
{
	
}

Widget_Slider4::~Widget_Slider4()
{
	
}

void Widget_Slider4::Construct(string sliderName, float* values, bool useSync, float min, float max)
{
	Super::Construct(sliderName, values, useSync, min, max);

	for (float& val : _preValue)
	{
		val = *values++;
	}
}

void Widget_Slider4::Tick()
{
	if (ImGui::SliderFloat4(_sliderName.c_str(), _values, _min, _max, "%.1f"))
	{
		if (_bUseSync)
			HandleSync();
	}
	ImGui::SameLine();
	ImGui::Checkbox(_syncLabel.c_str(), &_bUseSync);
}

void Widget_Slider4::HandleSync()
{
	float diff = 0.f;
	
	for (int32 i = 0; i < 3; ++i)
	{
		if (_preValue[i] != _values[i])
		{
			diff = _values[i];
			break;
		}
	}
	
	for (int32 i = 0; i < 3; ++i)
	{
		_preValue[i] = diff;
		_values[i] = diff;
	}

	// 위 코드 SIMD로 처리하면 이런식이 될 것 같음
	//__m128 a = _mm_loadu_ps(_preValue.data());
	//__m128 b = _mm_loadu_ps(_values);
	//__m128 delta = _mm_sub_ps(a, b);
	//__m128 absDelta = _mm_andnot_ps(_mm_set1_ps(-0.0f), delta);
	//
	//__m128 threshold = _mm_set1_ps(0.01f);
	//__m128 cmp = _mm_cmpgt_ps(absDelta, threshold); // delta > threshold
	//
	//__m128 src = _mm_loadu_ps(_values);
	//__m128 v = _mm_set1_ps(*_values);
	//__m128 result = _mm_blend_ps(v, src, 0b1000);
	//_mm_storeu_ps(_values, result);
	//_mm_storeu_ps(_preValue.data(), result);
}
