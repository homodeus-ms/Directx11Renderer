#pragma once
#include "Pawn.h"
#include "Utils/Delegate.h"

DECLARE_MULTICAST_DELEGATE_OneParam(OnCameraLookChanged, const Vec3&);

class CameraActor : public Pawn
{
	using Super = Pawn;

public:
	CameraActor();
	CameraActor(shared_ptr<Pawn> owner);
	virtual ~CameraActor();

	virtual void Construct() override;
	virtual void EnableController(bool bEnable) override;
	virtual void SetTransformChanged(bool bChanged) override;

	OnCameraLookChanged _onCameraLookChanged;

private:
	weak_ptr<Pawn> _owner;
};

