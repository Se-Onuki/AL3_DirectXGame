#pragma once
#include "Entity.h"

class PlayerBullet : public Entity {

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

public:
	PlayerBullet();
	~PlayerBullet();

	void Init(Model* model, const Vector3& position, const Vector3& velocity);
	void Update() override;

	const bool& IsDead() const { return isDead_; }
};