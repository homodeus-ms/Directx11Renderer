#pragma once
#include "Structs/LightTypes.h"

class LightManager;
class LightActor;
class SpotLight;
class PointLight;
class Widget_Slider4XN;
class Widget_Slider3;
class Widget_Slider4;

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
	void ShowFPS();
	void DrawGlobalLightWidget();
	void DrawSpotLightWidget();
	void DrawPointLightWidget();
	void DrawActorControlWidget();
	void DrawCubeMap();
	void DrawShowDebugShadowMapSelector();
	void OnActorRegistered(weak_ptr<Actor> actor);
	void HandleMoveLight(const shared_ptr<LightActor>& light);
	void SetLightOrbitMode(shared_ptr<LightActor> light, bool bOnOff);
	void MoveLightOrbitOnSpaceKeyDown(shared_ptr<LightActor> light);
	

	const vector<string> DEFAULT_VAR_NAMES = { "Ambient", "Diffuse", "Specular", "Emissive" };
	enum { MAX_SHOW_ACTOR_COUNT = 3 };

	template<typename T>
	void TickSliders(const vector<T>& sliders)
	{
		for (auto slider : sliders)
			slider->Tick();
	}

	shared_ptr<LightActor> CreateLightControlWidget(ELightType lightType, int32 index);
	void RemoveLightControlWidget(ELightType lightType, int32 index);
	
	bool _bVariableCached = false;

	// Light Move Common
	shared_ptr<LightActor> _orbitActor = nullptr;
	bool _bIsOrbitOn = false;
	bool _bRadiusGet = false;
	bool _orbitSpeed = 1.2f;
	float _orbitAngle = 0.f;
	float _radius = 10.f;
	
	// Global Light
	vector<Widget_Slider4*> _globalLightSliders{};
	Widget_Slider3* _globalLightPosition{};
	shared_ptr<LightActor> _globalLightActor;
	ImVec2 _globalLightLabelSize;
	bool _bGlobalLightOn = true;
	bool _bGlobalLightOnMove = false;

	// Spot Light
	const char* SPOT_BUTTON_LABELS[3]{};
	const char* CHECK_MOVE_SPOT_LABELS[3]{"Move Spot Light1 ", "Move Spot Light2 ", "Move Spot Light3 "};
	array<shared_ptr<LightActor>, 3> _spotLightActors;
	array<vector<Widget_Slider4*>, 3> _spotLightSliders1{};
	array<vector<Widget_Slider3*>, 3> _spotLightSliders2{};
	bool _bSpotLightOnMove[3]{};
	ImVec2 _spotLightLabelSize;
	
	// Point Light
	const char* POINT_BUTTON_LABELS[3]{};
	const char* CHECK_MOVE_POINT_LABELS[3]{ "Move Point Light1 ", "Move Point Light2 ", "Move Point Light3 " };
	array<shared_ptr<LightActor>, 3> _pointLightActors;
	array<vector<Widget_Slider4*>, 3> _pointLightSliders1{};
	array<vector<Widget_Slider3*>, 3> _pointLightSliders2{};
	bool _bPointLightOnMove[3]{};
	ImVec2 _pointLightLabelSize;

	// Actor - Material
	const uint32 _EMaterialTypeCount = GetEMaterialTypeCount();
	const vector<string> MATERIAL_TYPE_NAMES = { "Default", "RimLight", "Toon", "Monotone"};
	vector<ImVec2> _materialButtonLabelSize;
	vector<weak_ptr<Actor>> _actors;
	vector<vector<shared_ptr<Material>>> _actorMaterials;
	vector<vector<vector<Widget_Slider4*>>> _actorSliders{};
	vector<EMaterialType> _materialType;

	// Env Setting
	shared_ptr<Actor> _cubeMap = nullptr;
	// TEMP 3-CubeMap
	enum { CACHED_CUBE_MAP_COUNT = 3 };
	int32 _currCubeMapIndex = -1;
	string CUBE_MAP_NAMES[CACHED_CUBE_MAP_COUNT] = { "Skybox", "Night", "Snow" };
	ImVec2 _cubeMapLabelSize;
	bool _bEnvLightingOn = false;

	// Show Debug Shadow Map
	bool _bShowDebugShadowMap = false;
	bool _bShowDebugCubeMap = false;
};

