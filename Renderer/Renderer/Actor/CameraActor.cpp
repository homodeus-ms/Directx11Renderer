#include "pch.h"
#include "CameraActor.h"
#include "Components/Transform.h"
#include "Components/Controller.h"
#include "Components/CameraComponent.h"

CameraActor::CameraActor(shared_ptr<Pawn> owner)
	: Super()
	, _owner(owner)
{
	_moveSpeed = 10.f;
}

CameraActor::~CameraActor()
{
}

void CameraActor::Construct()
{
	Super::Construct();
	
	GetTransform()->SetWorldPosition(Vec3(0.f, 0.f, -3.f));
	AddComponent(make_shared<CameraComponent>());
	GetController()->SetMoveMode(EMoveMode::CameraMove);
}
