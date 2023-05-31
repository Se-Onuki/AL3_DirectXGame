#pragma once
#include "Header/Entity/Entity.h"
#include "input.h"

class GameScene;

class Player : public Entity {
	Input* input_ = nullptr;
	Vector2 MovementLimit = {34.5f, 18.5f};
	const float MoveSpeed = 0.2f;
	const float kRotSpeed = 0.02f;
	const float kBulletSpeed = 0.5f;

	GameScene* gameScene_ = nullptr;

	void Attack();

public:
	void SetGameScene(GameScene* gameScene);
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }
	void Init(Model* model, const uint32_t& textureHandle) override;

	void OnCollision() override;

	void Update() override;
	void Draw(const ViewProjection& Vp) override;

	Player();
	~Player();
};