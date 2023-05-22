#pragma once
#include "Entity.h"

enum class Phase {
	Approach, // 接近
	Leave,    // 離脱
};

class BaseEnemyState;

class Enemy : public Entity {

	BaseEnemyState* state_;

	void ApproachState();
	void LeaveState();


public:
	void ChangeState(BaseEnemyState* newState);
	const static float DefaultSpeed;
	Enemy();
	~Enemy();

	void Init(const std::string& modelKey, const uint32_t& textureHandle, const Vector3& position);
	void Update() override;
};

class BaseEnemyState {
public:
	virtual ~BaseEnemyState(){};
	Enemy* enemy_ = nullptr;
	virtual void Update() = 0;
};

class EnemyStateApproach : public BaseEnemyState {
public:
	void Update() override;
};

class EnemyStateLeave : public BaseEnemyState {
public:
	void Update() override;
};