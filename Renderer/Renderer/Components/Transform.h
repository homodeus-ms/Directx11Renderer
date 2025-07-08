#pragma once
#include "Component.h"

class Transform : public Component, public enable_shared_from_this<Transform>
{
	using Super = Component;

public:
	Transform();
	virtual ~Transform();

	virtual void BeginPlay() override;
	virtual void Tick() override;

	void UpdateTransform();

	// Local
	Vec3 GetLocalScale() { return _localScale; }
	void SetLocalScale(const Vec3& localScale) { _localScale = localScale; MarkDirty(); }
	Vec3 GetLocalRotation() { return _localRotation; }
	void SetLocalRotation(const Vec3& localRotation) { _localRotation = localRotation; MarkDirty(); }
	Vec3 GetLocalPosition() { return _localPosition; }
	void SetLocalPosition(const Vec3& localPosition) { _localPosition = localPosition; MarkDirty(); }

	// World
	Vec3 GetWorldScale();
	void SetWorldScale(const Vec3& worldScale);
	Vec3 GetWorldRotation();
	void SetWorldRotation(const Vec3& worldRotation);
	Vec3 GetWorldPosition();
	void SetWorldPosition(const Vec3& worldPosition);

	Vec3 GetRight();
	Vec3 GetUp();
	Vec3 GetLook();

	Matrix GetWorldMatrix();

	
	bool HasParent() { return _parent.lock() != nullptr; }
	shared_ptr<Transform> GetParent() { return _parent.lock(); }
	void SetParent(shared_ptr<Transform> parent);

	const vector<shared_ptr<Transform>>& GetChildren() { return _children; }
	

private:
	void AddChild(shared_ptr<Transform> child);
	void MarkDirty();
	void UpdateRotationMatrix();
	void EnsureWorldValueUpdated();
	void RefreshWorldScalePositionValues();
	void RefreshWorldRotationValue();

	weak_ptr<Transform> _parent;
	vector<shared_ptr<Transform>> _children;

	Vec3 _localScale = { 1.f, 1.f, 1.f };
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localPosition = { 0.f, 0.f, 0.f };

	Vec3 _worldScale;
	Vec3 _worldRotation;
	Vec3 _worldPosition;

	Matrix _matToParent = Matrix::Identity;
	Matrix _matToWorld = Matrix::Identity;

	Matrix _matToParentRotation = Matrix::Identity;
	Matrix _matToWorldRotation = Matrix::Identity;

	bool _bDirty = true;
	bool _bWorldScalePositionRefreshed = false;
	bool _bWorldRotationRefreshed = false;
};

