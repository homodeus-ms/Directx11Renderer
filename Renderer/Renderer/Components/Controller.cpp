#include "pch.h"
#include "Controller.h"
#include "Actor/Pawn.h"
#include "Components/Transform.h"

Controller::Controller()
	: Super(ComponentType::Controller)
{
}

Controller::~Controller()
{
}

void Controller::BeginPlay()
{
	Super::BeginPlay();

	_owner = static_pointer_cast<Pawn>(GetOwner());
	_moveSpeed = _owner.lock()->GetMoveSpeed();
}

void Controller::Tick()
{
	Super::Tick();

	switch (_moveMode)
	{
	case EMoveMode::NotUsing:
		return;
	case EMoveMode::PawnMove:
	{
		// TEMP
		// MovePawn();
		return;
	}
	case EMoveMode::CameraMove:
	{
		MoveCamera();
		return;
	}
	default:
		LOG(Warning, "EMoveMode is not valid");
	}
}

void Controller::MovePawn()
{
	float dt = TIME_MANAGER->GetDeltaTime();
	_moveDelta = GetOwnerTransform()->GetWorldPosition();

	if (INPUT_MANAGER->ButtonHold(KeyType::A))
	{
		_moveDelta.x -= _moveSpeed * dt;
		_bDirty = true;
	}
	else if (INPUT_MANAGER->ButtonHold(KeyType::D))
	{
		_moveDelta.x += _moveSpeed * dt;
		_bDirty = true;
	}
	else if (INPUT_MANAGER->ButtonHold(KeyType::W))
	{
		_moveDelta.y += _moveSpeed * dt;
		_bDirty = true;
	}
	else if (INPUT_MANAGER->ButtonHold(KeyType::S))
	{
		_moveDelta.y -= _moveSpeed * dt;
		_bDirty = true;
	}

	HandleDirty();
}

void Controller::MoveCamera()
{
	float dt = TIME_MANAGER->GetDeltaTime();
	_moveDelta = GetOwnerTransform()->GetWorldPosition();

	// WSAD : Move 
	if (INPUT_MANAGER->ButtonHold(KeyType::W))
	{
		Vec3 look = GetOwnerTransform()->GetLook();
		_moveDelta += GetOwnerTransform()->GetLook() * _moveSpeed * dt;
		_bDirty = true;
	}

	if (INPUT_MANAGER->ButtonHold(KeyType::S))
	{
		_moveDelta -= GetOwnerTransform()->GetLook() * _moveSpeed * dt;
		_bDirty = true;
	}

	if (INPUT_MANAGER->ButtonHold(KeyType::A))
	{
		_moveDelta -= GetOwnerTransform()->GetRight() * _moveSpeed * dt;
		_bDirty = true;
	}

	if (INPUT_MANAGER->ButtonHold(KeyType::D))
	{
		_moveDelta += GetOwnerTransform()->GetRight() * _moveSpeed * dt;
		_bDirty = true;
	}

	/*shared_ptr<Transform> transform = GetOwnerTransform();
	transform->SetWorldPosition(moveDelta);
	_owner.lock()->SetTransformChanged(true);*/

	HandleDirty();


	// QEZC : Rotation
	if (INPUT_MANAGER->ButtonHold(KeyType::Q))
	{
		Vec3 rotation = GetOwnerTransform()->GetLocalRotation();
		rotation.x += dt * _cameraRotateSpeed;
		GetOwnerTransform()->SetLocalRotation(rotation);

		_bDirty = true;
	} 
	else if (INPUT_MANAGER->ButtonHold(KeyType::E))
	{
		Vec3 rotation = GetOwnerTransform()->GetLocalRotation();
		rotation.x -= dt * _cameraRotateSpeed;
		GetOwnerTransform()->SetLocalRotation(rotation);

		_bDirty = true;
	}
	else if (INPUT_MANAGER->ButtonHold(KeyType::Z))
	{
		Vec3 rotation = GetOwnerTransform()->GetLocalRotation();
		rotation.y += dt * _cameraRotateSpeed;
		GetOwnerTransform()->SetLocalRotation(rotation);

		_bDirty = true;
	}
	else if (INPUT_MANAGER->ButtonHold(KeyType::C))
	{
		Vec3 rotation = GetOwnerTransform()->GetLocalRotation();
		rotation.y -= dt * _cameraRotateSpeed;
		GetOwnerTransform()->SetLocalRotation(rotation);

		_bDirty = true;
	}

	if (_bDirty)
		_owner.lock()->SetTransformChanged(true);
}


void Controller::HandleDirty()
{
	if (_bDirty)
	{
		shared_ptr<Transform> transform = GetOwnerTransform();
		transform->SetWorldPosition(_moveDelta);

		_owner.lock()->SetTransformChanged(true);

		_bDirty = false;
	}
	else
	{
		_owner.lock()->SetTransformChanged(false);
	}
}
