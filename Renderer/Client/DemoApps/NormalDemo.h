#pragma once
#include "Engine/IExecute.h"

class ClientPawn;

class NormalDemo : public IExecute
{
public:
	virtual void Construct() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render() override;

private:
	
	vector<shared_ptr<Actor>> _actors;

	shared_ptr<class Actor> _cameraActor;
	
};

