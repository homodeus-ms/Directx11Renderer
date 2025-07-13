#pragma once
#include "Engine/IExecute.h"

class ClientPawn;

class LoadMaterialDemo : public IExecute
{
public:
	virtual void Construct() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render() override;

private:
	shared_ptr<ClientPawn> _clientPawn;
	shared_ptr<class Actor> _cameraActor;
};

