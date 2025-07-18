#include "pch.h"
#include "CameraActor.h"
#include "Components/Transform.h"
#include "Components/Controller.h"
#include "Components/CameraComponent.h"

CameraActor::CameraActor()
	: Super(EActorType::CameraActor)
{
}

CameraActor::CameraActor(shared_ptr<Pawn> owner)
	: Super(EActorType::CameraActor)
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
	GetTransform()->SetWorldPosition(Vec3(0.f, 0.f, -15.f));
	//GetTransform()->SetLocalRotation(Vec3(0.4f, 0.f, 0.f));
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

void CameraActor::SetTransformChanged(bool bChanged)
{
	Super::SetTransformChanged(bChanged);

	if (bChanged)
		_onCameraLookChanged.Broadcast(GetTransform()->GetLook());
}
