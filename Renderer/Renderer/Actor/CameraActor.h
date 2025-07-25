#pragma once
#include "Pawn.h"
#include "Utils/Delegate.h"

DECLARE_MULTICAST_DELEGATE_OneParam(OnCameraLookChanged, const Vec3&);

class CameraComponent;

class CameraActor : public Pawn
{
	using Super = Pawn;

public:
	CameraActor(const string& name = "Camera");
	CameraActor(shared_ptr<Pawn> owner, const string& name = "Camera");
	virtual ~CameraActor();

	virtual void Construct() override;
	virtual void EnableController(bool bEnable) override;
	virtual void SetTransformChanged(bool bChanged) override;

	shared_ptr<CameraComponent> GetCameraComponent() { return _cameraComponent; }
	const Matrix& GetV();
	const Matrix& GetP();
	const Matrix& GetCameraVP();
	OnCameraLookChanged _onCameraLookChanged;

private:
	shared_ptr<CameraComponent> _cameraComponent;
	weak_ptr<Pawn> _owner;
};

