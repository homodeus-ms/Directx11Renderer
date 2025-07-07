#pragma once
#include "Engine/IExecute.h"

class ClientActor;

class TriangleDemo : public IExecute
{
public:

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render() override;

private:
	shared_ptr<ClientActor> _clientActor;
};

