#pragma once
#include "Entity.h"

enum class Phase {
	Approach, // 接近
	Leave,    // 離脱
};

class Enemy : public Entity {
	// フェーズ
	Phase phase_ = Phase::Approach;
	const static float DefaultSpeed;

	void ApproachState();
	void LeaveState();

	static void (Enemy::*StateTable[])();

public:
	Enemy();
	~Enemy();

	void Init(const std::string& modelKey, const uint32_t& textureHandle, const Vector3& position);
	void Update() override;
};