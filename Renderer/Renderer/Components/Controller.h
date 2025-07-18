#pragma once
#include "Component.h"

enum class EMoveMode
{
	NotUsing,
	PawnMove,
	CameraMove,
};

class Pawn;

class Controller : public Component
{
	using Super = Component;

public:
	Controller();
	virtual ~Controller();

	virtual void BeginPlay() override;
	virtual void Tick() override;

	EMoveMode GetMoveMode() const { return _moveMode; }
	void SetMoveMode(EMoveMode moveMode) { _moveMode = moveMode; }
	

private:

	void MovePawn();
	void MoveCamera();
	void HandleDirty();

	weak_ptr<Pawn> _owner;
	EMoveMode _moveMode = EMoveMode::PawnMove;
	Vec3 _moveDelta{};
	float _moveSpeed = 7.f;
	float _cameraRotateSpeed = 2.f;
	bool _bDirty = false;

};

