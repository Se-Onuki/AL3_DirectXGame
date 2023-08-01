#pragma once

#include "input.h"
#include <WorldTransform.h>
#include <stdint.h>

class PlayerBullet;
struct ViewProjection;
class Model;

class Player {
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;

	WorldTransform worldTransformOrigin_;

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformLeft_;
	WorldTransform worldTransformRight_;

	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	const ViewProjection* viewProjection_ = nullptr;

	const float moveSpeed_ = 0.5f;

	float floatingParameter_ = 0.f;
	const float originPos = 0.5f;
	void InitFloatingGimmick();
	void UpdateFloatingGimmick();

public:
	void Init(
	    Model* const modelBody, Model* const modelHead, Model* const modelL_arm,
	    Model* const modelR_arm, const uint32_t& textureHandle);
	void Update();
	void Draw(const ViewProjection& Vp) const;
	void SetViewProjection(const ViewProjection* const viewProjection) {
		viewProjection_ = viewProjection;
	}

	const WorldTransform& GetWorldTransform() const { return worldTransformOrigin_; }

	Player();
	~Player();
};