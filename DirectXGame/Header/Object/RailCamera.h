#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class Rail;
class RailCamera {
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	Rail* rail_ = nullptr;
	Vector3 Direction_;

	Vector3 CalcFacing(const Vector3& directionVector) {
		Vector3 rotate{0, 0, 0};
		rotate.y = std::atan2(directionVector.x, directionVector.z);
		rotate.x = std::atan2(
		    -directionVector.y,
		    std::sqrt(std::powf(directionVector.x, 2) + std::powf(directionVector.z, 2)));
		return rotate;
	}

public:
	void Init(const Vector3& position, const Vector3& rotate);
	void Update();

	void SetRail(Rail* const rail) { rail_ = rail; }

	const ViewProjection& GetViewProjection() const { return viewProjection_; }
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }
	// void SetTranslation(const Vector3& translation) { worldTransform_.translation_ = translation;
	// } void SetRotation(const Vector3& rotation) { worldTransform_.rotation_ = rotation; }
};