#include "pch.h"
#include "EngineClientFlowManager.h"
#include "UI/Controller/GUIController.h"

EngineClientFlowManager::~EngineClientFlowManager()
{
	
}

void EngineClientFlowManager::BeginPlay()
{
	_guiController = make_shared<GUIController>();
	SCENE_MANAGER->_onBeforeSceneConstruct.BindObject(shared_from_this(), &EngineClientFlowManager::ExecutePreSceneConstructFuncs);
	SCENE_MANAGER->_onBeforeSceneBeginPlay.BindObject(shared_from_this(), &EngineClientFlowManager::ExecutePreSceneBeginPlayFuncs);
	SCENE_MANAGER->_onAfterSceneBeginPlay.BindObject(shared_from_this(), &EngineClientFlowManager::ExecutePostSceneBeginPlayFuncs);
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

void EngineClientFlowManager::ExecutePostSceneBeginPlayFuncs()
{
	AddPostSceneBeginPlayFuncs.Broadcast();
}
