#include "pch.h"
#include "LeftWindowController.h"
//#include "Managers/LightManager.h"
#include "Actor/LightActor.h"
#include "Components/LightComponent/LightComponent.h"
#include "Components/LightComponent/DirectionalLight.h"
#include "Components/LightComponent/SpotLight.h"
#include "Components/LightComponent/PointLight.h"
#include "UI/Sliders/Widget_Slider4.h"
#include "UI/Sliders/Widget_Slider3.h"
#include "Actor/BulbActor.h"


LeftWindowController::LeftWindowController()
{
	SPOT_BUTTON_LABELS[0] = "SpotLight1 ";
	SPOT_BUTTON_LABELS[1] = "SpotLight2 ";
	SPOT_BUTTON_LABELS[2] = "SpotLight3 ";

	POINT_BUTTON_LABELS[0] = "PointLight1 ";
	POINT_BUTTON_LABELS[1] = "PointLight2 ";
	POINT_BUTTON_LABELS[2] = "PointLight3 ";
}

LeftWindowController::~LeftWindowController()
{
	for (auto ptr : _globalLightSliders)
	{
		SAFE_DELETE(ptr);
	}

	for (auto vec : _spotLightSliders1)
	{
		for (auto ptr : vec)
			SAFE_DELETE(ptr);
	}

	for (auto vec : _spotLightSliders2)
	{
		for (auto ptr : vec)
			SAFE_DELETE(ptr);
	}

	for (auto vec : _pointLightSliders1)
	{
		for (auto ptr : vec)
			SAFE_DELETE(ptr);
	}

	for (auto vec : _pointLightSliders2)
	{
		for (auto ptr : vec)
			SAFE_DELETE(ptr);
	}
}

void LeftWindowController::BeginPlay()
{
	SCENE->_onLightManagerCreated.BindObject(shared_from_this(), &LeftWindowController::OnLightManagerCreatedCallback);
}

void LeftWindowController::CreateLeftWindow()
{
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_Always);
	ImGui::SetNextWindowSize({ GUI_MainWindowSizeX, GUI_MainWindowSizeY }, ImGuiCond_Always);
	ImGui::Begin("LeftWindow", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove);

	// Variables Cache Once
	if (!_bVariableCached)
		CacheVariables();

	ImGui::Spacing();
	ImGui::Separator();

	// Global Light
	if (_globalLightActor)
	{
		ImGui::Text("Global Light");
		ImGui::SameLine();
		ImGui::Checkbox("OnOff", &_bGlobalLightOn);
		if (_bPreGlobalLightOn != _bGlobalLightOn)
			OnGlobalLightOnOffChanged();

		TickSliders(_globalLightSliders);
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Spot Light
	for (uint32 i = 0; i < MAX_ADDITIONAL_LIGHT_COUNT; ++i)
	{
		bool bActive = (_spotLightActors[i] != nullptr);

		if (ImGui::Selectable(SPOT_BUTTON_LABELS[i], bActive, 0, _spotLightLabelSize))
		{
			if (!_spotLightActors[i])
				CreateLightControlWidget(ELightType::Spot, i);
			else
				RemoveLightControlWidget(ELightType::Spot, i);
		}

		if (bActive && ImGui::CollapsingHeader(SPOT_BUTTON_LABELS[i]))
		{
			TickSliders(_spotLightSliders1[i]);
			TickSliders(_spotLightSliders2[i]);
		}
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Point Light
	for (uint32 i = 0; i < MAX_ADDITIONAL_LIGHT_COUNT; ++i)
	{
		bool bActive = (_pointLightActors[i] != nullptr);

		if (ImGui::Selectable(POINT_BUTTON_LABELS[i], bActive, 0, _pointLightLabelSize))
		{
			if (!_pointLightActors[i])
				CreateLightControlWidget(ELightType::Point, i);
			else
				RemoveLightControlWidget(ELightType::Point, i);
		}

		if (bActive && ImGui::CollapsingHeader(POINT_BUTTON_LABELS[i]))
		{
			TickSliders(_pointLightSliders1[i]);
			TickSliders(_pointLightSliders2[i]);
		}
	}

	ImGui::End();
}

void LeftWindowController::OnLightManagerCreatedCallback()
{
	CreateLightControlWidget(ELightType::Directional, 0);
}

void LeftWindowController::OnGlobalLightOnOffChanged()
{
	SCENE->TurnGlobalLightOnOff(_bGlobalLightOn);
	_bPreGlobalLightOn = _bGlobalLightOn;
}

shared_ptr<LightActor> LeftWindowController::CreateLightControlWidget(ELightType lightType, int32 index)
{
	switch (lightType)
	{
	case ELightType::Directional:
	{
		assert(_globalLightActor == nullptr);

		_globalLightActor = SCENE->GetGlobalLight();
		DirectionalLightDesc* desc = static_cast<DirectionalLightDesc*>(_globalLightActor->GetDesc());
		
		vector<float*> colorPtrs
		{
			(float*)&desc->ambient,
			(float*)&desc->diffuse,
			(float*)&desc->specular,
			(float*)&desc->emissive,
		};

		for (int32 i = 0; i < 4; ++i)
		{
			_globalLightSliders.push_back(new Widget_Slider4("Directional"));
			_globalLightSliders[i]->Construct(DEFAULT_VAR_NAMES[i], colorPtrs[i]);
		}

		return _globalLightActor;
	}
	case ELightType::Spot:
	{
		assert(index < MAX_ADDITIONAL_LIGHT_COUNT);
		assert(_spotLightActors[index] == nullptr);

		shared_ptr<LightActor> lightActor = SCENE->AddSpotLight();
		_spotLightActors[index] = lightActor;
		SpotLightDesc* desc = static_cast<SpotLightDesc*>(lightActor->GetDesc());

		vector<float*> variables = {
			(float*)&desc->ambient,
			(float*)&desc->diffuse,
			(float*)&desc->specular,
			(float*)&desc->emissive,
		};

		for (int32 i = 0; i < 4; ++i)
		{
			_spotLightSliders1[index].push_back(new Widget_Slider4("Spot"));
			_spotLightSliders1[index][i]->Construct(DEFAULT_VAR_NAMES[i], variables[i], true, 0.f, 2.f);
		}

		_spotLightSliders2[index].push_back(new Widget_Slider3("Spot"));
		_spotLightSliders2[index][0]->Construct("Position", (float*)&desc->position, false, -20.f, 20.f);
		_spotLightSliders2[index][0]->SetShowFloatDigit(0);
		_spotLightSliders2[index].push_back(new Widget_Slider3("Spot"));
		_spotLightSliders2[index][1]->Construct("Direction", (float*)&desc->direction, false, -1.f, 1.f);
		

		return lightActor;
	}
	case ELightType::Point:
	{
		assert(index < MAX_ADDITIONAL_LIGHT_COUNT);
		assert(_pointLightActors[index] == nullptr);

		shared_ptr<LightActor> lightActor = SCENE->AddPointLight();
		_pointLightActors[index] = lightActor;
		PointLightDesc* desc = static_cast<PointLightDesc*>(lightActor->GetDesc());

		vector<float*> variables = {
			(float*)&desc->ambient,
			(float*)&desc->diffuse,
			(float*)&desc->specular,
			(float*)&desc->emissive,
		};

		for (int32 i = 0; i < 4; ++i)
		{
			_pointLightSliders1[index].push_back(new Widget_Slider4("Point"));
			_pointLightSliders1[index][i]->Construct(DEFAULT_VAR_NAMES[i], variables[i], true, 0.f, 2.f);
		}

		_pointLightSliders2[index].push_back(new Widget_Slider3("Point"));
		_pointLightSliders2[index][0]->Construct("Position", (float*)&desc->position, false, -20.f, 20.f);
		_pointLightSliders2[index][0]->SetShowFloatDigit(0);

		return lightActor;
	}
	default:
	{
		assert(false);
		return nullptr;
	}
	}


	return nullptr;
}

void LeftWindowController::RemoveLightControlWidget(ELightType lightType, int32 index)
{
	assert(index < MAX_ADDITIONAL_LIGHT_COUNT);
	assert(lightType == ELightType::Spot || lightType == ELightType::Point);

	if (lightType == ELightType::Spot)
	{
		SCENE->RemoveLight(_spotLightActors[index]);
		_spotLightActors[index] = nullptr;

		{
			vector<Widget_Slider4*>& vec = _spotLightSliders1[index];
			for (auto ptr : vec)
			{
				SAFE_DELETE(ptr);
				vec.clear();
			}
		}

		{
			vector<Widget_Slider3*>& vec = _spotLightSliders2[index];
			for (auto ptr : vec)
			{
				SAFE_DELETE(ptr);
				vec.clear();
			}
		}
	}
	else
	{
		SCENE->RemoveLight(_pointLightActors[index]);
		_pointLightActors[index] = nullptr;

		{
			vector<Widget_Slider4*>& vec = _pointLightSliders1[index];
			for (auto ptr : vec)
			{
				SAFE_DELETE(ptr);
				vec.clear();
			}
		}

		{
			vector<Widget_Slider3*>& vec = _pointLightSliders2[index];
			for (auto ptr : vec)
			{
				SAFE_DELETE(ptr);
				vec.clear();
			}
		}
	}
}

void LeftWindowController::CacheVariables()
{
	_spotLightLabelSize = ImGui::CalcTextSize(SPOT_BUTTON_LABELS[0]);
	_pointLightLabelSize = ImGui::CalcTextSize(POINT_BUTTON_LABELS[0]);
	_bVariableCached = true;
}


