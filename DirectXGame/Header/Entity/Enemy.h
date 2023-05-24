#pragma once
#include "Entity.h"
#include "Header/Timer/Timer.h"

namespace EnemyState {
class Base;
}
class EnemyBullet;
class Player;

class Enemy : public Entity {

	EnemyState::Base* state_;
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	const uint16_t maxCooltime_ = 50;
	std::list<std::unique_ptr<FunctionTimer>> fireTimerList_;
	const float kBulletSpeed = 0.5f;
	Player* player_ = nullptr;

public:
	void SetPlayer(Player* player) { player_ = player; };

	void Fire();
	void FireAndInit();
	void SetFireTimer();
	void FireTimerDelete();
	void ChangeState(EnemyState::Base* newState);
	const static float DefaultSpeed;
	Enemy();
	~Enemy();

	void Init(Model* model, const uint32_t& textureHandle, const Vector3& position);
	void Update() override;
	void Draw(const ViewProjection& Vp) override;
};

namespace EnemyState {
class Base {
public:
	virtual ~Base(){};
	Enemy* enemy_ = nullptr;
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;
};

class Approach : public EnemyState::Base {
public:
	void Enter() override;
	void Update() override;
	void Exit() override;
};

class Leave : public EnemyState::Base {
public:
	void Enter() override;
	void Update() override;
	void Exit() override;
};

} // namespace EnemyState

class EnemyBullet : public Entity {

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

public:
	EnemyBullet() {}
	virtual ~EnemyBullet() {}

	void Init(Model* model, const Vector3& position, const Vector3& velocity);
	void Update() override;

	const bool& IsDead() const { return isDead_; }
};