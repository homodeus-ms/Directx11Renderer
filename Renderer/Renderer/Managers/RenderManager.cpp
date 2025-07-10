#include "pch.h"
#include "RenderManager.h"
#include "Actor/Actor.h"

void RenderManager::Render(vector<shared_ptr<Actor>>& actors)
{
	for (const shared_ptr<Actor>& actor : actors)
	{
		actor->Render();
	}
}
