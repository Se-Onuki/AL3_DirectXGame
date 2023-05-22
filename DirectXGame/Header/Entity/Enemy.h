#pragma once
#include "Entity.h"

namespace EnemyState {
class Base;
}

class Enemy : public Entity {

	EnemyState::Base* state_;

public:
	void ChangeState(EnemyState::Base* newState);
	const static float DefaultSpeed;
	Enemy();
	~Enemy();

	void Init(const std::string& modelKey, const uint32_t& textureHandle, const Vector3& position);
	void Update() override;
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