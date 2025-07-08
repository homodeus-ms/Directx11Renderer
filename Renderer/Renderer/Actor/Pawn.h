#pragma once
#include "Actor.h"

class Controller;

class Pawn : public Actor
{
	using Super = Actor;

public:
	Pawn();
	virtual ~Pawn();

	virtual void Construct() override;
	virtual void BeginPlay() override;

	virtual void AttachFollowCamera(shared_ptr<Actor> cameraActor, bool bKeyInputForCameraMovement = true) override;

	float GetMoveSpeed() const { return _moveSpeed; }

protected:
	shared_ptr<Controller> GetController();

	void AddController();
	bool _bKeyInputForCamera = false;
	float _moveSpeed = 3.f;
};

