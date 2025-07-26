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
	
	_shadowMap->CreateShadowMap(shadowedActors, lightActors); // ������ �� ����
	
	GRAPHICS->RenderBegin();  // ���� ��ü�� �׸��� RenderTarget, viewport�� ����

	UpdateCommonDatas(lightActors);  // Global������: ī�޶�VP, ���Ǵ� ��� ������ VP, ������� SRV�� 

	// Draw ShadowMap Debug
	if (SCENE->ShouldDrawDebugShadowMap())
	{
		shadowMapDebugActor->Render();    // ������ �� ����׿�
	}

	// Render
	for (const shared_ptr<Actor>& actor : drawTargets)
		actor->Render();
	
	SHADER_PARAM_MANAGER->CleanUpDatasAfterRender();
}

void RenderManager::UpdateCommonDatas(const vector<shared_ptr<LightActor>>& lightActors)
{
	// �������� ����ϴ� cbuffer�� ���� : cbuffer�� �ö� ���� �� ������ ��
	// ���� ��� ���Ͱ� �������� ����� ������ �� �����ӿ� �� ���� GPU�� ���ε��ϸ� ��
	SHADER_PARAM_MANAGER->PushGlobalData(SCENE->GetCurrCameraV(), SCENE->GetCurrCameraP());

	// ShadowMap ���� �����͵�
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

	// Light, EnvLight source, ShadowMap SRV��
	SHADER_PARAM_MANAGER->BindCommonResources();
}
