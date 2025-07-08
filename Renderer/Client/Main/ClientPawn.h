#pragma once
#include "Actor/Pawn.h"

class ClientPawn : public Pawn
{
	using Super = Pawn;

public:
	ClientPawn();
	virtual ~ClientPawn();

	virtual void BeginPlay() override;
	virtual void Tick() override;

private:
	Vec3 _translation = Vec3(0.f, 0.f, 0.f);

	Matrix _world = Matrix::Identity;
	Matrix _view = Matrix::Identity;
	Matrix _projection = Matrix::Identity;

	shared_ptr<ConstantBuffer<TransformData>> _constBuffer;
	TransformData _transformData; 
};

