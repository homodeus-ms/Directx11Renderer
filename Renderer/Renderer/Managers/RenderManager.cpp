#include "pch.h"
#include "RenderManager.h"
#include "Actor/Actor.h"
#include "Actor/LightActor.h"
#include "Graphics/RenderPass/ShadowMap.h"
#include "Managers/SceneManager.h"
#include "Managers/ShaderParameterManager.h"
#include "Components/CameraComponent.h"

void RenderManager::Render(vector<shared_ptr<Actor>>& actors)
{
	vector<shared_ptr<LightActor>> lightActors;
	vector<shared_ptr<Actor>> shadowedActors;
	shared_ptr<Actor> shadowMapDebugActor;
	vector<shared_ptr<Actor>> drawTargets;

	for (shared_ptr<Actor> actor : actors)
	{
		if (actor->GetActorType() == EActorType::DebugActor)
		{
			shadowMapDebugActor = actor;
			continue;
		}

		if (actor->GetActorType() == EActorType::LightActor)
		{
			lightActors.push_back(static_pointer_cast<LightActor>(actor));
		}
		else if (actor->IsCastShadowedActor())
		{
			shadowedActors.push_back(actor);
		}
		drawTargets.push_back(actor);
	}
	
	_shadowMap->CreateShadowMap(shadowedActors, lightActors); // 섀도우 맵 생성
	
	GRAPHICS->RenderBegin();  // 실제 물체를 그리는 RenderTarget, viewport등 셋팅

	UpdateCommonDatas(lightActors);  // Global데이터: 카메라VP, 사용되는 모든 조명의 VP, 섀도우맵 SRV등 

	// Draw ShadowMap Debug
	if (SCENE->ShouldDrawDebugShadowMap())
	{
		shadowMapDebugActor->Render();    // 섀도우 맵 디버그용
	}

	// Render
	for (const shared_ptr<Actor>& actor : drawTargets)
		actor->Render();
	
	SHADER_PARAM_MANAGER->CleanUpDatasAfterRender();
}

void RenderManager::UpdateCommonDatas(const vector<shared_ptr<LightActor>>& lightActors)
{
	// 공통으로 사용하는 cbuffer들 셋팅 : cbuffer에 올라간 값은 값 유지가 됨
	// 따라서 모든 액터가 공통으로 사용할 값들은 한 프레임에 한 번만 GPU로 업로드하면 됨
	SHADER_PARAM_MANAGER->PushGlobalData(SCENE->GetCurrCameraV(), SCENE->GetCurrCameraP());

	// ShadowMap 관련 데이터들
	//for (int32 i = 0; i < lightActors.size(); ++i)
	//{
	//	shared_ptr<LightActor> light = lightActors[i];
	//	ELightType type = light->GetLightType();
	//
	//	if (type == ELightType::Directional || type == ELightType::Spot)
	//	{
	//		SHADER_PARAM_MANAGER->PushShadowMapSRV(light->GetShadowSRVInfo());
	//	}
	//	else
	//	{
	//		SHADER_PARAM_MANAGER->PushShadowCubeMapSRV(light->GetShadowSRVInfo());
	//	}
	//}

	//SHADER_PARAM_MANAGER->PushShadowMapSRV(_shadowMap->GetShadowMapSRVBindingInfo());
	//SHADER_PARAM_MANAGER->PushShadowCubeMapSRV(_shadowMap->GetPointLightShadowMapSRVBindingInfo());

	// Light, EnvLight source, ShadowMap SRV등
	SHADER_PARAM_MANAGER->BindCommonResources();
}
