#include "pch.h"
#include "CameraComponent.h"
#include "Transform.h"

Matrix CameraComponent::S_MatView = Matrix::Identity;
Matrix CameraComponent::S_MatProjection = Matrix::Identity;

CameraComponent::CameraComponent()
	: Super(EComponentType::Camera)
{
	_width = static_cast<float>(RENDERER->GetGameDesc().width);
	_height = static_cast<float>(RENDERER->GetGameDesc().height);
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
	S_MatView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
	S_MatProjection = ::XMMatrixPerspectiveFovLH(_fov, _width / _height, _near, _far);
}
