#include "pch.h"
#include "ReflectComponent.h"
#include "Actor/Actor.h"
#include "Transform.h"

ReflectComponent::ReflectComponent()
	: Super(EComponentType::ReflectComponent)
{
}

ReflectComponent::~ReflectComponent()
{
}

void ReflectComponent::Construct()
{
	Super::Construct();

	_pos = GetOwner()->GetTransform()->GetWorldPosition();
	GetOwner()->_onTransformChanged.BindObject(shared_from_this(), &ReflectComponent::OnTransformChanged);
}

Matrix ReflectComponent::GetReflectMatrix()
{
	if (!_bReflectMatrixCalculated)
	{
		_faceNormal.Normalize();
		_reflectMatrix = Matrix::CreateReflection({ _pos, _faceNormal });
		_bReflectMatrixCalculated = true;
		return _reflectMatrix;
	}

	if (_bDirty)
	{
		_pos = GetOwner()->GetTransform()->GetWorldPosition();
		Vec3 rotation = GetOwner()->GetTransform()->GetLocalRotation();
		_faceNormal *= rotation;
		_faceNormal.Normalize();
		_reflectMatrix = Matrix::CreateReflection({ _pos, _faceNormal });
		_bDirty = false;
	}

	return _reflectMatrix;
}

void ReflectComponent::OnTransformChanged()
{
	_bDirty = true;
}
