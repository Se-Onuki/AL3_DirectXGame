#pragma once
#include "Entity.h"

class PlayerBullet : public Entity {

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

protected:
	const float kBulletSpeed = 0.75f;

public:
	PlayerBullet();
	virtual ~PlayerBullet();
	virtual void OnCollision(const Collider* const other) override;

	void Init(Model* model, const Vector3& position, const Vector3& velocity);
	virtual void Update() override;

	const bool& IsDead() const { return isDead_; }

	void SetFacing();
};

class Enemy;
class HomingPlayerBullet : public PlayerBullet {
	using PlayerBullet::PlayerBullet;
	static std::list<Collider*> targetList_;
	Collider* target_ = nullptr;

public:
	void OnCollision(const Collider* const other) override;
	void Update() override;
	void SetTarget(Collider* const enemy);
};