#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
class RailCamera {
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

public:
	void Init(const Vector3& position, const Vector3& rotate);
	void Update();

	const ViewProjection& GetViewProjection() const { return viewProjection_; }
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
};