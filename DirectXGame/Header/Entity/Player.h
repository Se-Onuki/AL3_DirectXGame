#pragma once

#include "Math.hpp"
#include "input.h"
#include <WorldTransform.h>
#include <optional>
#include <stdint.h>

#include "BaseCharacter.h"

class PlayerBullet;
struct ViewProjection;
class Model;

class Player : public BaseCharacter {

	enum class Behavior {
		kRoot,   // 通常状態
		kAttack, // 攻撃力
	};

	Behavior behavior_ = Behavior::kRoot;

	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformLeft_;
	WorldTransform worldTransformRight_;

	WorldTransform worldTransformWeapon_;

	Input* input_ = nullptr;
	const ViewProjection* viewProjection_ = nullptr;

	const float moveSpeed_ = 0.5f;

	float floatingParameter_ = 0.f;
	const float originPos = 0.5f;

	int32_t floatingCycle_ = 120u;
	float floatingCycleRange_ = 0.2f;
	float floatingSwayHand_ = 30 * Angle::Dig2Rad;

	int32_t attackCycle_ = 90u;
	float attackStartAngle_ = 0.f * Angle::Dig2Rad;
	float attackSwingAngle_ = 130.f * Angle::Dig2Rad;
	float attackClampAngle_ = 110.f * Angle::Dig2Rad;

	void ApplyClobalVariables();

	void InitFloatingGimmick();
	void UpdateFloatingGimmick();

	void BehaviorRootInit();
	void BehaviorRootUpdate();

	void BehaviorAttackInit();
	void BehaviorAttackUpdate();

	void UpdateWorldMatrix();

public:
	void Init(const std::unordered_map<std::string, Model*>& model) override;
	void Update() override;
	void Draw(const ViewProjection& Vp) const override;
	void SetViewProjection(const ViewProjection* const viewProjection) {
		viewProjection_ = viewProjection;
	}

	Player();
	~Player() override;
};