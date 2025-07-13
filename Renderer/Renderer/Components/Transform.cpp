#include "pch.h"
#include "Transform.h"

Transform::Transform()
	: Super(EComponentType::Transform)
{
}

Transform::~Transform()
{
}

void Transform::BeginPlay()
{
	Super::BeginPlay();
}

void Transform::Tick()
{
	Super::Tick();
}

Vec3 ToEulerAngles(Quaternion q)
{
	Vec3 angles;

	// roll (x-axis rotation)
	double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	angles.x = static_cast<float>(std::atan2(sinr_cosp, cosr_cosp));

	// pitch (y-axis rotation)
	double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	angles.y = static_cast<float>(2 * std::atan2(sinp, cosp) - 3.14159f / 2);

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angles.z = static_cast<float>(std::atan2(siny_cosp, cosy_cosp));

	return angles;
}

void Transform::UpdateTransform()
{
	/*Matrix matScale = Matrix::CreateScale(_localScale);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	_matToParent = matScale * matRotation * matTranslation;

	if (HasParent())
	{
		_matToWorld = _matToParent * _parent.lock()->GetWorldMatrix();
	}
	else
	{
		_matToWorld = _matToParent;
	}

	Quaternion quat;
	_matToWorld.Decompose(_worldScale, quat, _worldPosition);
	_worldRotation = ToEulerAngles(quat);

	for (const shared_ptr<Transform>& child : _children)
		child->UpdateTransform();*/


	Matrix matScale = Matrix::CreateScale(_localScale);
	// For Non Uniform Scaling, Seperate keep rotation value
	UpdateRotationMatrix();
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);
	
	_matToParent = matScale * _matToParentRotation * matTranslation;

	if (HasParent()) 
	{
		_matToWorld = _matToParent * _parent.lock()->GetWorldMatrix();
	}
	else
	{
		_matToWorld = _matToParent;
		_bDirty = false;
	}

	// Children
	for (const shared_ptr<Transform>& child : _children)
	{
		if (child->_bDirty)
			child->UpdateTransform();
	}
}

void Transform::UpdateRotationMatrix()
{
	_matToParentRotation = Matrix::CreateRotationY(_localRotation.y);
	_matToParentRotation *= Matrix::CreateRotationX(_localRotation.x);
	_matToParentRotation *= Matrix::CreateRotationZ(_localRotation.z);

	if (HasParent())
		_matToWorldRotation = _matToParentRotation * _parent.lock()->_matToWorldRotation;
	else
		_matToWorldRotation = _matToParentRotation;
}

Matrix Transform::GetWorldMatrix()
{
	if (_bDirty)
	{
		UpdateTransform();
		_bDirty = false;
	}

	return _matToWorld;
}

Vec3 Transform::GetWorldScale()
{
	EnsureWorldValueUpdated();
	return _worldScale;
}

Vec3 Transform::GetWorldRotation()
{
	EnsureWorldValueUpdated();
	return _worldRotation;
}

Vec3 Transform::GetWorldPosition()
{
	EnsureWorldValueUpdated();
	return _worldPosition;
}

void Transform::SetWorldScale(const Vec3& worldScale)
{
	if (HasParent())
	{
		Vec3 parentScale = _parent.lock()->GetWorldScale();
		Vec3 scale = worldScale;
		scale.x /= parentScale.x;
		scale.y /= parentScale.y;
		scale.z /= parentScale.z;
		SetLocalScale(scale);
	}
	else
	{
		SetLocalScale(worldScale);
	}
}

void Transform::SetWorldRotation(const Vec3& worldRotation)
{
	/*if (HasParent())
	{
		Quaternion worldQuat = Quaternion::CreateFromYawPitchRoll(worldRotation.y, worldRotation.x, worldRotation.z);
		Quaternion parentQuat = Quaternion::CreateFromRotationMatrix(_parent.lock()->GetWorldMatrix());
		Quaternion localQuat = XMQuaternionMultiply(XMQuaternionInverse(parentQuat), worldQuat);

		Vec3 localEuler = ToEulerAngles(localQuat);
		SetLocalRotation(localEuler);
	}
	else
		SetLocalRotation(worldRotation);*/

	if (HasParent())
	{
		Matrix inverseMatrix = _parent.lock()->GetWorldMatrix().Invert();

		Vec3 rotation;
		rotation.TransformNormal(worldRotation, inverseMatrix);

		SetLocalRotation(rotation);
	}
	else
		SetLocalRotation(worldRotation);
}

void Transform::SetWorldPosition(const Vec3& worldPosition)
{
	if (HasParent())
	{
		Matrix worldToParentLocalMatrix = _parent.lock()->GetWorldMatrix().Invert();

		Vec3 position;
		position.Transform(worldPosition, worldToParentLocalMatrix);

		SetLocalPosition(position);
	}
	else
	{
		SetLocalPosition(worldPosition);
	}
}

Vec3 Transform::GetRight()
{
	if (_bDirty)
		UpdateTransform();

	return _matToWorldRotation.Right();
}

Vec3 Transform::GetUp()
{
	if (_bDirty)
		UpdateTransform();

	return _matToWorldRotation.Up();
}

Vec3 Transform::GetLook()
{
	if (_bDirty)
		UpdateTransform();

	return _matToWorldRotation.Backward();
}

void Transform::SetParent(shared_ptr<Transform> parent)
{
	if (_parent.lock() == parent)
		return;

	if (shared_ptr<Transform> oldParent = _parent.lock())
	{
		vector<shared_ptr<Transform>>& children = oldParent->_children;
		children.erase(std::remove(children.begin(), children.end(), shared_from_this()), children.end());
	}

	_parent = parent;
	parent->AddChild(shared_from_this());

	MarkDirty();
}

void Transform::AddChild(shared_ptr<Transform> child)
{
	_children.push_back(child);
}

void Transform::MarkDirty()
{
	if (_bDirty)
		return;

	_bDirty = true;
	_bWorldScalePositionRefreshed = false;
	_bWorldRotationRefreshed = false;

	for (auto& child : _children)
	{
		if (!child->_bDirty)
			child->MarkDirty();
	}
}

void Transform::EnsureWorldValueUpdated()
{
	if (_bDirty)
		UpdateTransform();

	if (!_bWorldScalePositionRefreshed)
		RefreshWorldScalePositionValues();

	if (!_bWorldRotationRefreshed)
		RefreshWorldRotationValue();
}

void Transform::RefreshWorldScalePositionValues()
{
	Quaternion quat;
	_matToWorld.Decompose(_worldScale, quat, _worldPosition);
	_bWorldScalePositionRefreshed = true;
}

void Transform::RefreshWorldRotationValue()
{
	Quaternion quat = Quaternion::CreateFromRotationMatrix(_matToWorldRotation);
	_worldRotation = ToEulerAngles(quat);
	_bWorldRotationRefreshed = true;
}

