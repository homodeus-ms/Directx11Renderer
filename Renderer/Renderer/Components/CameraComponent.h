#pragma once
#include "Component.h"

enum class EProjectionType
{
	Perspective, // 원근 투영
	Orthographic, // 직교 투영
};

class CameraComponent : public Component
{
	using Super = Component;
public:
	CameraComponent();
	virtual ~CameraComponent();

	virtual void Tick() override;

	void UpdateMatrix();

	void SetNear(float value) { _near = value; }
	void SetFar(float value) { _far = value; }
	void SetFOV(float value) { _fov = value; }
	void SetWidth(float value) { _width = value; }
	void SetHeight(float value) { _height = value; }
	
	const Matrix GetV() { return _matView; }
	const Matrix GetP() { return _matProjection; }
	const Matrix GetVP() { return _matView * _matProjection; }

private:
	Matrix _matView = Matrix::Identity;
	Matrix _matProjection = Matrix::Identity;

	float _near = 1.f;
	float _far = 1000.f;
	float _fov = XM_PI / 4.f;
	float _width = 0.f;
	float _height = 0.f;

public:
	//static Matrix S_MatView;
	//static Matrix S_MatProjection;
};
