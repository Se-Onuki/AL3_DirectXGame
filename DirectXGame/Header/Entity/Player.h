#pragma once
#include "Header/Entity/Entity.h"
#include "input.h"

class GameScene;
class Sprite;
class Targeting;

class Player : public Entity {
	Input* input_ = nullptr;
	Vector2 MovementLimit = {20.f, 10.5f};
	const float MoveSpeed = 0.2f;
	const float kRotSpeed = 0.02f;
	const float kBulletSpeed = 0.5f;

	GameScene* gameScene_ = nullptr;

	WorldTransform worldTransform3DReticle_;

	Sprite* sprite2DReticle_ = nullptr;
	Targeting* targeting_ = nullptr;
	Collider* enemyTarget_ = nullptr;

	void Attack();

public:
	void SetGameScene(GameScene* gameScene);
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }
	void Init(Model* model, const uint32_t& textureHandle) override;

	void OnCollision(const Collider* const other) override;

	void Update(const ViewProjection& Vp);
	void Draw(const ViewProjection& Vp) override;
	void DrawUI() const;

	Player();
	~Player();
};