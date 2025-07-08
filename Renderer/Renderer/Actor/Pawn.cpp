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

	if (_bHasCamera && _bKeyInputForCamera)
	{
		if (const shared_ptr<Controller>& myController = GetController())
		{
			myController->SetMoveMode(EMoveMode::NotUsing);
		}
	}
}

void Pawn::AttachFollowCamera(shared_ptr<Actor> cameraActor, bool bKeyInputForCameraMovement)
{
	Super::AttachFollowCamera(cameraActor, bKeyInputForCameraMovement);

	_bKeyInputForCamera = bKeyInputForCameraMovement;
}


shared_ptr<Controller> Pawn::GetController()
{
	return static_pointer_cast<Controller>(GetFixedComponent(ComponentType::Controller));
}

void Pawn::AddController()
{
	shared_ptr<Component> controller = make_shared<Controller>();
	AddComponent(controller);
}
