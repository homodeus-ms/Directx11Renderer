#pragma once
#include "Structs/LightTypes.h"

class LightManager;
class LightActor;
class SpotLight;
class PointLight;
class Widget_Slider4XN;
class Widget_Slider3;
class Widget_Slider4;

//struct LightControlWidget
//{
//	shared_ptr<LightActor> lightActor{};
//	ELightType lightType{};
//	LightDesc* desc{};
//	Widget_Slider4XN* widget{};
//	vector<Widget_Slider3*> addedWidget{};
//};

class LeftWindowController : public enable_shared_from_this<LeftWindowController>
{
	enum { MAX_ADDITIONAL_LIGHT_COUNT = 3 };
public:
	LeftWindowController();
	virtual ~LeftWindowController();
	
	void BeginPlay();

	void CreateLeftWindow();
	void OnLightManagerCreatedCallback();

private:
	void CacheVariables();

	template<typename T>
	void TickSliders(const vector<T>& sliders)
	{
		for (auto slider : sliders)
			slider->Tick();
	}

	void OnGlobalLightOnOffChanged();
	shared_ptr<LightActor> CreateLightControlWidget(ELightType lightType, int32 index);
	void RemoveLightControlWidget(ELightType lightType, int32 index);

	const vector<string> DEFAULT_VAR_NAMES = { "Ambient", "Diffuse", "Specular", "Emissive" };
	bool _bVariableCached = false;

	// Global Light
	vector<Widget_Slider4*> _globalLightSliders{};
	shared_ptr<LightActor> _globalLightActor;
	bool _bPreGlobalLightOn = true;
	bool _bGlobalLightOn = true;

	// Spot Light
	const char* SPOT_BUTTON_LABELS[3]{};
	array<shared_ptr<LightActor>, 3> _spotLightActors;
	array<vector<Widget_Slider4*>, 3> _spotLightSliders1{};
	array<vector<Widget_Slider3*>, 3> _spotLightSliders2{};

	ImVec2 _spotLightLabelSize;
	
	// Point Light
	const char* POINT_BUTTON_LABELS[3]{};
	array<shared_ptr<LightActor>, 3> _pointLightActors;
	array<vector<Widget_Slider4*>, 3> _pointLightSliders1{};
	array<vector<Widget_Slider3*>, 3> _pointLightSliders2{};

	ImVec2 _pointLightLabelSize;

};

