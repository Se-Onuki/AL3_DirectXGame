#pragma once
#include "Header/Entity/Entity.h"
#include "input.h"

class PlayerBullet;

class Player : public Entity {
	Input* input_ = nullptr;
	const ViewProjection* viewProjection_ = nullptr;

	const float moveSpeed_ = 0.5f;

public:
	void Init(Model* model, const uint32_t& textureHandle) override;
	void Update() override;
	void Draw(const ViewProjection& Vp) override;
	void SetViewProjection(const ViewProjection* const viewProjection) {
		viewProjection_ = viewProjection;
	}

	Player();
	~Player();
};