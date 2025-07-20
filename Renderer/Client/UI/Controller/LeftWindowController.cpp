#include "pch.h"
#include "LeftWindowController.h"
//#include "Managers/LightManager.h"
#include "Actor/LightActor.h"
#include "Components/LightComponent/LightComponent.h"
#include "Components/LightComponent/DirectionalLight.h"
#include "Components/LightComponent/SpotLight.h"
#include "Components/LightComponent/PointLight.h"
#include "Resource/Material.h"
#include "UI/Sliders/Widget_Slider4.h"
#include "UI/Sliders/Widget_Slider3.h"
#include "Actor/BulbActor.h"
#include "Utils/Utils.h"


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

	for (const vector<vector<Widget_Slider4*>>& actorSlider : _actorSliders)
	{
		for (const vector<Widget_Slider4*>& matSlider : actorSlider)
		{
			for (Widget_Slider4* ptr : matSlider)
				SAFE_DELETE(ptr);
		}
	}
}

void LeftWindowController::BeginPlay()
{
	SCENE->_onLightManagerCreated.BindObject(shared_from_this(), &LeftWindowController::OnLightManagerCreatedCallback);
	SCENE->_onRenderedActorRegistered.BindObject(shared_from_this(), &LeftWindowController::OnActorRegistered);
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
	
	ShowFPS();
	DrawGlobalLightWidget();
	DrawSpotLightWidget();
	DrawPointLightWidget();
	DrawActorControlWidget();
	DrawCubeMap();

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

		shared_ptr<LightActor> lightActor = SCENE->AddPointLightOrNull();
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

	for (const string& name : MATERIAL_TYPE_NAMES)
	{
		const ImVec2 size = ImGui::CalcTextSize(name.c_str());
		_materialButtonLabelSize.push_back(size);
	}

	_cubeMapLabelSize = ImGui::CalcTextSize(CUBE_MAP_NAMES[0].c_str());

	_bVariableCached = true;

}

void LeftWindowController::ShowFPS()
{
	ImGui::Spacing();
	ImGui::Separator();

	string fpsStr = "fps : ";
	float fps = TIME_MANAGER->GetFps();
	fpsStr += std::to_string(fps);
	
	ImGui::Text(fpsStr.c_str());
}

void LeftWindowController::DrawGlobalLightWidget()
{
	ImGui::Spacing();
	ImGui::Separator();

	if (_globalLightActor)
	{
		ImGui::Text("Global Light");
		ImGui::SameLine();
		ImGui::Checkbox("OnOff", &_bGlobalLightOn);
		if (_bPreGlobalLightOn != _bGlobalLightOn)
			OnGlobalLightOnOffChanged();

		TickSliders(_globalLightSliders);
	}
}

void LeftWindowController::DrawSpotLightWidget()
{
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
}

void LeftWindowController::DrawPointLightWidget()
{
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
			{
				CreateLightControlWidget(ELightType::Point, i);
			}
			else
			{
				RemoveLightControlWidget(ELightType::Point, i);
			}
		}

		if (bActive && ImGui::CollapsingHeader(POINT_BUTTON_LABELS[i]))
		{
			TickSliders(_pointLightSliders1[i]);
			TickSliders(_pointLightSliders2[i]);
		}
	}
}

void LeftWindowController::DrawActorControlWidget()
{
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	uint32 actorCount = static_cast<uint32>(_actors.size());
	actorCount = actorCount < MAX_SHOW_ACTOR_COUNT ? actorCount : MAX_SHOW_ACTOR_COUNT;

	for (uint32 i = 0; i < actorCount; ++i)
	{
		string buttonName = _actors[i].lock()->GetName();

		if (ImGui::CollapsingHeader(buttonName.c_str()))
		{
			// A, D, S, E 컨트롤
			// 하나의 Slider로 액터의 모든 Material ADSE값을 같은 값으로 변경
			float* changedVal = nullptr;
			
			for (uint32 ADSE = 0; ADSE < 4; ++ADSE)
			{
				changedVal = _actorSliders[i][0][ADSE]->TickSlider();

				if (changedVal)
				{
					// Material0의 변화값을 1번부터~ 나머지 Material에 적용
					for (int32 j = 1; j < _actorSliders[i].size(); ++j)
					{
						_actorSliders[i][j][ADSE]->UpdateValue(changedVal);
					}
				}
				changedVal = nullptr;
			}
			

			ImGui::Spacing();
			ImGui::Separator();

			// Material Type 컨트롤
			ImGui::Text("< Material Type Select >");

			for (uint32 j = 0; j < _EMaterialTypeCount; ++j)
			{
				MaterialDesc* desc = _actorMaterials[i][0]->GetDesc(); // 대표 하나
				EMaterialType currType = desc->MaterialType;
				string typeString = GetEMaterialTypeString(currType);
				bool bActive = typeString == MATERIAL_TYPE_NAMES[j];

				if (ImGui::Selectable(MATERIAL_TYPE_NAMES[j].c_str(), bActive, 0, _materialButtonLabelSize[j]))
				{
					EMaterialType newType = static_cast<EMaterialType>(j);
					_actors[i].lock()->ChangeMaterialType(newType);
				}
				ImGui::SameLine(0, 10.f);
			}
		}

		ImGui::Spacing();
	}
}

void LeftWindowController::DrawCubeMap()
{
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Text("- Environment Map -");

	for (uint32 i = 0; i < CACHED_CUBE_MAP_COUNT; ++i)
	{
		bool bActive = _currCubeMapIndex == i;

		if (ImGui::Selectable(CUBE_MAP_NAMES[i].c_str(), bActive, 0, _cubeMapLabelSize))
		{
			if (!bActive)
			{
				SCENE->CreateEnvironment(Utils::ToWString(CUBE_MAP_NAMES[i]));
				_currCubeMapIndex = i;
				_bEnvLightingOn = true;
			}
		}
		
		ImGui::SameLine();
	}

	ImGui::Text("   ");
	ImGui::SameLine();

	if (ImGui::Checkbox("EnvLight On", &_bEnvLightingOn))
	{
		SCENE->TurnEnvLightOnOff(_bEnvLightingOn);
	}
}

void LeftWindowController::OnActorRegistered(weak_ptr<Actor> actor)
{
	string actorName = actor.lock()->GetName();
	if (Utils::IsStartWith(actorName, "CubeMap"))
		return;

	_actors.push_back(actor);

	const vector<shared_ptr<Material>>& materials = actor.lock()->GetMaterials();

	_actorMaterials.push_back(materials);
	vector<vector<Widget_Slider4*>> actorMaterialSliders;

	// Sliders for Ambient, Diffuse, Specular, Emissive
	for (const shared_ptr<Material>& mat : materials)
	{
		MaterialDesc* desc = mat->GetDesc();

		vector<float*> variables = {
			(float*)&(desc->ambient),
			(float*)&(desc->diffuse),
			(float*)&(desc->specular),
			(float*)&(desc->emissive),
		};

		vector<Widget_Slider4*> sliders;

		for (int32 i = 0; i < 4; ++i)
		{
			Widget_Slider4* slider = new Widget_Slider4("Material");
			slider->Construct(DEFAULT_VAR_NAMES[i], variables[i], true, 0.f, 1.f);
			sliders.push_back(slider);
		}

		actorMaterialSliders.push_back(sliders);
	}

	_actorSliders.push_back(actorMaterialSliders);

	// Material type : Material이 여러개 있을 수 있지만 Material의 타입은 통일된다고 가정
	_materialType.push_back(materials[0]->GetDesc()->MaterialType);
}



