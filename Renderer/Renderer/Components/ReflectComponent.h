#pragma once
#include "Component.h"

class ReflectComponent : public Component, public enable_shared_from_this<ReflectComponent>
{
	using Super = Component;

public:
	ReflectComponent();
	virtual ~ReflectComponent();

	virtual void Construct() override;
	void SetFaceNormal(Vec3 faceNormal) { _faceNormal = faceNormal; }
	Matrix GetReflectMatrix();
	
	void OnTransformChanged();

private:
	Vec3 _pos;
	Vec3 _faceNormal = { 0.f, 0.f, -1.f };
	Matrix _reflectMatrix = Matrix::Identity;

	bool _bDirty = false;
	bool _bReflectMatrixCalculated = false;
};

