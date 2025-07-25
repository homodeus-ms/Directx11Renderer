#pragma once
#include "Actor/Pawn.h"

class ClientPawn : public Pawn
{
	using Super = Pawn;

public:
	ClientPawn(const string& name = "Pawn");
	virtual ~ClientPawn();

	virtual void BeginPlay() override;
	virtual void Tick() override;

private:
	Vec3 _translation = Vec3(0.f, 0.f, 0.f);

	Matrix _world = Matrix::Identity;
	Matrix _view = Matrix::Identity;
	Matrix _projection = Matrix::Identity;

	TransformData _transformData; 
};

