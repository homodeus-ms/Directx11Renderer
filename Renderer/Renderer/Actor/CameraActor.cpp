#include "pch.h"
#include "CameraActor.h"
#include "Components/Transform.h"
#include "Components/Controller.h"
#include "Components/CameraComponent.h"

CameraActor::CameraActor()
{
}

CameraActor::CameraActor(shared_ptr<Pawn> owner)
	: Super()
	, _owner(owner)
{
	
}

CameraActor::~CameraActor()
{
}

void CameraActor::Construct()
{
	Super::Construct();

	_moveSpeed = 10.f;
	GetTransform()->SetWorldPosition(Vec3(0.f, 0.f, -9.f));
	//GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
	AddComponent(make_shared<CameraComponent>());
	GetController()->SetMoveMode(EMoveMode::CameraMove);
}

void CameraActor::EnableController(bool bEnable)
{
	if (bEnable)
	{
		GetController()->SetMoveMode(EMoveMode::NotUsing);
	}
	else
	{
		GetController()->SetMoveMode(EMoveMode::CameraMove);
	}
}
