#include "pch.h"
#include "CameraActor.h"
#include "Components/Transform.h"
#include "Components/Controller.h"
#include "Components/CameraComponent.h"


CameraActor::CameraActor(const string& name)
	: Super(EActorType::CameraActor, name)
{
}

CameraActor::CameraActor(shared_ptr<Pawn> owner, const string& name)
	: Super(EActorType::CameraActor, name)
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
	GetTransform()->SetWorldPosition(Vec3(0.f, 3.f, -30.f));
	GetTransform()->SetLocalRotationByTargetLook(Vec3(0.f, -1.f, 10.f));
	_cameraComponent = make_shared<CameraComponent>();
	AddComponent(_cameraComponent);
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

const Matrix& CameraActor::GetV()
{
	return _cameraComponent->GetV();
}

const Matrix& CameraActor::GetP()
{
	return _cameraComponent->GetP();
}

const Matrix& CameraActor::GetCameraVP()
{
	return _cameraComponent->GetVP();
}
