#include "pch.h"
#include "PopUpController.h"
#include "Actor/Actor.h"
#include "Resource/Material/Material.h"
#include "Resource/Material/MaterialBase.h"
#include "Components/Transform.h"
#include "Utils/Utils.h"
#include "UI/Sliders/Widget_Slider4.h"

void PopUpController::BeginPlay()
{
	SCENE->_onRenderedActorRegistered.BindObject(shared_from_this(), &PopUpController::OnActorRegistered);
}

void PopUpController::CreatePopUpUI()
{
	ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ SubWindowX, SubWindowY });

	if (ImGui::BeginPopup("ActorControls"))
	{
		if (ImGui::Button("Close This Pop-up"))
		{
			_onPopUpClosed.Broadcast();
		}
		else
		{
			DrawActorControls();
		}

		ImGui::EndPopup();
	}
	else
	{
		_onPopUpClosed.Broadcast();
	}
}

void PopUpController::DrawActorControls()
{
	ImGui::Text("| PBR Material Actors |");

	for (uint32 i = 0; i < _pbrMaterialActors.size(); ++i)
	{
		string actorName = _pbrMaterialActors[i].lock()->GetName();

		if (ImGui::CollapsingHeader(actorName.c_str()))
		{
			const vector<shared_ptr<MaterialBase>> materials = _pbrMaterials[i];
			shared_ptr<MaterialBase> material0 = materials[0];
			MaterialDesc* desc0 = material0->GetDesc();

			//float* height = material0->GetHeightScalePtr();
			float* metallic = material0->GetMetallicValuePtr();
			float* roughness = material0->GetRoughnessValuePtr();

			//ImGui::SliderFloat(GetSliderName("Height", actorName, i).c_str(), height, 0.f, 5.f, "%.1f");
			ImGui::SliderFloat(GetSliderName("Metallic", actorName, i).c_str(), metallic, 0.f, 1.f, "%.1f");
			ImGui::SliderFloat(GetSliderName("Roughness", actorName, i).c_str(), roughness, 0.f, 1.f, "%.1f");

			// 나머지 다른 Material에도 변경된 값 적용
			for (uint32 j = 1; j < materials.size(); ++j)
			{
				shared_ptr<MaterialBase> otherMaterial = materials[j];
				MaterialDesc* otherDesc = otherMaterial->GetDesc();
				
				otherDesc->metallic = *metallic;
				otherDesc->roughness = *roughness;
			}
		}

		ImGui::Spacing();
	}

	ImGui::Text("| Reflect Actors |");
	for (uint32 i = 0; i < _reflectActors.size(); ++i)
	{
		string actorName = _reflectActors[i].lock()->GetName();

		if (ImGui::CollapsingHeader(actorName.c_str()))
		{
			shared_ptr<MaterialBase> material = _reflectMaterials[i];
			MaterialDesc& desc = material->GetMaterialDesc();
			
			float color[4];
			color[0] = desc.diffuse.x;
			color[1] = desc.diffuse.y;
			color[2] = desc.diffuse.z;
			color[3] = (desc.specular.x + desc.specular.y + desc.specular.z) / 3.f;

			if (ImGui::SliderFloat4(
				GetSliderName("Mirror Color", actorName, i).c_str(),
				color,
				0.f, 1.f, "%0.1f"))
			{
				desc.diffuse.x = color[0];
				desc.diffuse.y = color[1];
				desc.diffuse.z = color[2];
				desc.specular.x = color[3];
				desc.specular.y = color[3];
				desc.specular.z = color[3];
			}

			float* blendFactor = material->GetBlendFactorPtr();
			ImGui::SliderFloat(GetSliderName("Mirror Blend", actorName, i).c_str(), blendFactor, 0.f, 1.f, "%.2f");
		}

		ImGui::Spacing();
	}
}

void PopUpController::OnActorRegistered(weak_ptr<Actor> actor)
{
	string actorName = actor.lock()->GetName();
	if (Utils::IsStartWith(actorName, "CubeMap"))
		return;

	EActorType actorType = actor.lock()->GetActorType();
	if (actorType == EActorType::DebugActor || actorType == EActorType::LightActor)
		return;

	if (actorType == EActorType::ReflectActor)
	{
		// TEMP : Reflect Actor는 하나의 머테리얼만 사용한다고 가정함
		_reflectActors.push_back(actor);
		_reflectMaterials.push_back(actor.lock()->GetMaterials()[0]);
		return;
	}

	EMaterialType materialType = actor.lock()->GetMaterials()[0]->GetMaterialType();
	const vector<shared_ptr<MaterialBase>>& materials = actor.lock()->GetMaterials();
	switch (materialType)
	{
	case EMaterialType::Default:
		_basicMaterialActors.push_back(actor);
		_basicMaterials.push_back(materials);
		break;
	case EMaterialType::PBR:
		_pbrMaterialActors.push_back(actor);
		_pbrMaterials.push_back(materials);
		break;
	default:
		return;
	}

	return;


	// Sliders for Ambient, Diffuse, Specular, Emissive
	if (materialType == EMaterialType::Default)
	{
		for (const shared_ptr<MaterialBase>& mat : materials)
		{
			MaterialDesc* desc = mat->GetDesc();

			vector<float*> ADSE = {
				(float*)&(desc->ambient),
				(float*)&(desc->diffuse),
				(float*)&(desc->specular),
				(float*)&(desc->emissive),
			};

			vector<Widget_Slider4*> sliders;

			for (int32 i = 0; i < 4; ++i)
			{
				Widget_Slider4* slider = new Widget_Slider4("ADSE");
				slider->Construct(actorName, ADSE[i], true, 0.f, 1.f);
				sliders.push_back(slider);
			}

		}
	}
	else if (materialType == EMaterialType::PBR)
	{

	}
}

string PopUpController::GetSliderName(const string& shown, const string& op1, int op2)
{
	return string(shown + "##" + op1 + to_string(op2));
}
