#pragma once
#include "Header/Entity/Entity.h"
#include "input.h"

class PlayerBullet;

class Player : public Entity {
	Input* input_ = nullptr;


public:
	void Init(Model* model, const uint32_t& textureHandle) override;
	void Update() override;
	void Draw(const ViewProjection& Vp) override;

	Player();
	~Player();
};