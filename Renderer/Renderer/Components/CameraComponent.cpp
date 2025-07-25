#include "pch.h"
#include "CameraComponent.h"
#include "Transform.h"

//Matrix CameraComponent::S_MatView = Matrix::Identity;
//Matrix CameraComponent::S_MatProjection = Matrix::Identity;

CameraComponent::CameraComponent()
	: Super(EComponentType::Camera)
{
	_width = static_cast<float>(VIEW_X);
	_height = static_cast<float>(VIEW_Y);
}

CameraComponent::~CameraComponent()
{
}


void CameraComponent::Tick()
{
	Super::Tick();

	UpdateMatrix();
}

void CameraComponent::UpdateMatrix()
{
	Vec3 eyePosition = GetOwnerTransform()->GetWorldPosition();
	Vec3 focusPosition = eyePosition + GetOwnerTransform()->GetLook();
	Vec3 upDirection = GetOwnerTransform()->GetUp();
	_matView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
	_matProjection = ::XMMatrixPerspectiveFovLH(_fov, _width / _height, _near, _far);
}
