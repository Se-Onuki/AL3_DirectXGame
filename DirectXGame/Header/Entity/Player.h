#pragma once

#include "input.h"
#include <WorldTransform.h>
#include <stdint.h>

class PlayerBullet;
struct ViewProjection;
class Model;

class Player {
	WorldTransform worldTransform_;
	Model* modelBody_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	const ViewProjection* viewProjection_ = nullptr;

	const float moveSpeed_ = 0.5f;

public:
	void Init(Model* const modelBody, const uint32_t& textureHandle);
	void Update();
	void Draw(const ViewProjection& Vp) const;
	void SetViewProjection(const ViewProjection* const viewProjection) {
		viewProjection_ = viewProjection;
	}

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	Player();
	~Player();
};