#pragma once
#include "Actor.h"

class Controller;

class Pawn : public Actor
{
	using Super = Actor;

public:
	Pawn();
	Pawn(EActorType actorType);
	virtual ~Pawn();

	virtual void Construct() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;

	float GetMoveSpeed() const { return _moveSpeed; }
	virtual void EnableController(bool bEnable);

protected:
	shared_ptr<Controller> GetController();

	void AddController();
	
	float _moveSpeed = 3.f;

	bool _bEnableController = true;
};

