#include "pch.h"
#include "Pawn.h"
#include "Components/Controller.h"
#include "Components/CameraComponent.h"
#include "Components/Transform.h"

Pawn::Pawn()
	: Super()
{
	
}

Pawn::~Pawn()
{
}

void Pawn::Construct()
{
	Super::Construct();
	
	AddController();
}

void Pawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void Pawn::Tick()
{
	Super::Tick();

}

void Pawn::EnableController(bool bEnable)
{
	if (bEnable)
	{
		GetController()->SetMoveMode(EMoveMode::NotUsing);
	}
	else
	{
		GetController()->SetMoveMode(EMoveMode::PawnMove);
	}
}


shared_ptr<Controller> Pawn::GetController()
{
	return static_pointer_cast<Controller>(GetFixedComponent(EComponentType::Controller));
}

void Pawn::AddController()
{
	shared_ptr<Component> controller = make_shared<Controller>();
	AddComponent(controller);
}
