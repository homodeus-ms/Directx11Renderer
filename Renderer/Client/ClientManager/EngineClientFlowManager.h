#pragma once
#include  "Utils/Delegate.h"

// 사용 방법
// <Engine의 초기화 순서>
// app Construct -> SceneManager Construct -> SceneManager BeginPlay -> app BeginPlay
// app Construct에서 Scene이 돌아가기 전 필요한 것들을 모두 셋팅해야 함
// 엔진에 초기에 Binding 해야 하는 것들을 여기에 Add 하는 식으로 사용

DECLARE_MULTICAST_DELEGATE(AddPreSceneConstructFuncsDelegate);
DECLARE_MULTICAST_DELEGATE(AddPreSceneBeginPlayFuncsDelegate);
DECLARE_MULTICAST_DELEGATE(AddPostSceneBeginPlayFuncsDelegate);

class GUIController;

class EngineClientFlowManager : public enable_shared_from_this<EngineClientFlowManager>
{

public:
	~EngineClientFlowManager();

	// DemoApps에서 시작할 때 공통적으로 셋팅할 부분을 여기에 
	void BeginPlay();

	void ExecutePreSceneConstructFuncs();
	void ExecutePreSceneBeginPlayFuncs();
	void ExecutePostSceneBeginPlayFuncs();

	AddPreSceneConstructFuncsDelegate AddPreSceneConstructFuncs;
	AddPreSceneBeginPlayFuncsDelegate AddPreSceneBeginPlayFuncs;
	AddPostSceneBeginPlayFuncsDelegate AddPostSceneBeginPlayFuncs;

private:
	
	shared_ptr<GUIController> _guiController = nullptr;
};

