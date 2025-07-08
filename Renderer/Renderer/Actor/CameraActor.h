#pragma once
#include "Pawn.h"

class CameraActor : public Pawn
{
	using Super = Pawn;

public:

	CameraActor(shared_ptr<Pawn> owner);
	virtual ~CameraActor();

	virtual void Construct() override;


private:
	weak_ptr<Pawn> _owner;
};

