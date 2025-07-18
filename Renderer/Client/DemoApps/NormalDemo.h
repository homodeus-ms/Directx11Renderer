#pragma once
#include "Engine/IExecute.h"

DECLARE_MULTICAST_DELEGATE_OneParam(OnText, int32);

class ClientPawn;
class LightActor;

class NormalDemo : public IExecute
{
public:
	virtual void Construct() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render() override;

	OnText _onTextDelegate;

private:
	shared_ptr<ClientPawn> _clientPawn;
	shared_ptr<class Actor> _cameraActor;
	shared_ptr<LightActor> _spotLight1;
	shared_ptr<LightActor> _spotLight2;
	shared_ptr<LightActor> _spotLight3;
	shared_ptr<LightActor> _pointLight1;
	shared_ptr<LightActor> _pointLight2;
	shared_ptr<LightActor> _pointLight3;


	// TEMP
	shared_ptr<class GUIController> _guiController;

};

