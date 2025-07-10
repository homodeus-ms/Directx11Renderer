#pragma once
#include "Pawn.h"

class CameraActor : public Pawn
{
	using Super = Pawn;

public:
	CameraActor();
	CameraActor(shared_ptr<Pawn> owner);
	virtual ~CameraActor();

	virtual void Construct() override;

	virtual void EnableController(bool bEnable) override;

private:
	weak_ptr<Pawn> _owner;
};

