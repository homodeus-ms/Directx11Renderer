#pragma once

class Widget_SliderBase
{
public:
	Widget_SliderBase(const string& addedLabel);
	virtual ~Widget_SliderBase();

	virtual void Construct(string sliderName, float* values, bool useSync = true, float min = 0.f, float max = 1.f);
	virtual void Tick() {};
	virtual float* TickSlider() { return _values; }
protected:
	
	string _sliderName{};
	float* _values = nullptr;
	bool _bUseSync = false;
	float _min = 0.f;
	float _max = 0.f;

	string _addedLabel{};
	string _syncLabel = "Sync";
};

