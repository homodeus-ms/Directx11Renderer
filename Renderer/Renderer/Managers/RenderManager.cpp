#include "pch.h"
#include "RenderManager.h"
#include "Actor/Actor.h"
#include "Actor/LightActor.h"
#include "Graphics/RenderPass/ShadowMap.h"

void RenderManager::Render(vector<shared_ptr<Actor>>& actors)
{
	for (const shared_ptr<Actor>& actor : actors)
	{
		if (actor->GetActorType() == EActorType::LightActor)
		{
			// TEMP
			shared_ptr<LightActor> lightActor = static_pointer_cast<LightActor>(actor);
			if (lightActor->GetLightType() == ELightType::Directional)
			{
				_shadowMap->CreateShadowMap();
			}
		}
		actor->Render();
	}
}
