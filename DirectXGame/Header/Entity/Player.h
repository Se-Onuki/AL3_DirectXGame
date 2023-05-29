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

	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	void Attack();

public:

	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }
	void Init(Model* model, const uint32_t& textureHandle) override;

	void OnCollision() override;
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullet() const { return bullets_; }
	void Update() override;
	void Draw(const ViewProjection& Vp) override;

	Player();
	~Player();
};