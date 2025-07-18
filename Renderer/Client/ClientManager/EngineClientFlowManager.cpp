#include "pch.h"
#include "EngineClientFlowManager.h"
#include "UI/Controller/GUIController.h"

EngineClientFlowManager::~EngineClientFlowManager()
{
	
}

void EngineClientFlowManager::BeginPlay()
{
	_guiController = make_shared<GUIController>();
	_guiController->BeginPlay();
}

void EngineClientFlowManager::ExecutePreSceneConstructFuncs()
{
	AddPreSceneConstructFuncs.Broadcast();
}

void EngineClientFlowManager::ExecutePreSceneBeginPlayFuncs()
{
	AddPreSceneBeginPlayFuncs.Broadcast();
}
