#pragma once
#include "Engine/IExecute.h"
#include "Utils/Delegate.h"

DECLARE_MULTICAST_DELEGATE_OneParam(OnTest, int32);

class ClientPawn;
class LightActor;
class BulbActor;

class LoadMaterialDemo : public IExecute
{
public:
	virtual void Construct() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render() override;

	OnTest _onTestDelegate;

private:
	shared_ptr<ClientPawn> _clientPawn;
	shared_ptr<class Actor> _cameraActor;
	shared_ptr<LightActor> _spotLight1;
	shared_ptr<LightActor> _spotLight2;
	shared_ptr<LightActor> _spotLight3;
	shared_ptr<LightActor> _pointLight1;
	shared_ptr<LightActor> _pointLight2;
	shared_ptr<LightActor> _pointLight3;
	shared_ptr<BulbActor> _bulb;
	

	// TEMP
	shared_ptr<class GUIController> _guiController;
};

