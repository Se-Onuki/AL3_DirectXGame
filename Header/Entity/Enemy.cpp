#include "Enemy.h"
#include <imgui.h>

const float Enemy::DefaultSpeed = 0.2f;
void (Enemy::*Enemy::StateTable[])() = {	// staticの関数ポインタ配列の変数に、関数のアドレスを定義する。
	&Enemy::ApproachState,
	&Enemy::LeaveState
};

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Init(
    const std::string& modelKey, const uint32_t& textureHandle, const Vector3& position) {
	Entity::Init(modelKey, textureHandle);
	worldTransform_.translation_ = position;
}

void Enemy::Update() {
	uint32_t stateNumber = static_cast<uint32_t>(phase_);
	(this->*StateTable[stateNumber])();


	worldTransform_.UpdateMatrix();
	const Vector3& translation = worldTransform_.translation_;
	ImGui::Begin("Enemy");
	ImGui::Text("x: %.2f, y: %.2f, z: %.2f", translation.x, translation.y, translation.z);
	ImGui::End();
}

void Enemy::ApproachState() {
	// 移動
	worldTransform_.translation_ += Vector3{0.f, 0.f, -1.f}.Nomalize() * DefaultSpeed;
	if (worldTransform_.translation_.z <= 0.f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveState() {
	// 移動
	worldTransform_.translation_ +=
	    (worldTransform_.translation_ - Vector3::zero()).Nomalize() * DefaultSpeed;
}
