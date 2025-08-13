#include "pch.h"
#include "ClientPawn.h"

ClientPawn::ClientPawn(const string& name)
	: Super(name)
{
}

ClientPawn::ClientPawn(EActorType type, const string& name)
	: Super(type, name)
{
}

ClientPawn::~ClientPawn()
{
}

void ClientPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ClientPawn::Tick()
{
	Super::Tick();

}
