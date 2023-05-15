#pragma once
#include "Header/Entity/Entity.h"
#include "input.h"

class PlayerBullet;

class Player : public Entity {
	Input* input_ = nullptr;
	Vector2 MovementLimit = {34.5f, 18.5f};
	const float MoveSpeed = 0.2f;
	const float kRotSpeed = 0.02f;
	const float kBulletSpeed = 0.5f;

	std::list<PlayerBullet*> bullets_;

	void Attack();

public:
	void Init(const std::string& modelKey, const uint32_t& textureHandle) override;
	void Update() override;
	void Draw(const ViewProjection& Vp) override;

	Player();
	~Player();
};