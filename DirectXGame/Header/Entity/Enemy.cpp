#include "Enemy.h"
#include <imgui.h>

const float Enemy::DefaultSpeed = 0.2f;

void Enemy::ChangeState(EnemyState::Base* newState) {
	delete state_;
	state_ = newState;
	state_->enemy_ = this;
}

Enemy::Enemy() { ChangeState(new EnemyState::Approach()); }

Enemy::~Enemy() {}

void Enemy::Init(
    Model* model, const uint32_t& textureHandle, const Vector3& position) {
	Entity::Init(model, textureHandle);
	worldTransform_.translation_ = position;
}

void Enemy::Update() {
	state_->Update();

	worldTransform_.UpdateMatrix();
	const Vector3& translation = worldTransform_.translation_;
	ImGui::Begin("Enemy");
	ImGui::Text("x: %.2f, y: %.2f, z: %.2f", translation.x, translation.y, translation.z);
	ImGui::End();
}

void EnemyState::Approach::Enter() {}

void EnemyState::Approach::Update() {
	// 移動
	enemy_->AddPosition(Vector3{0.f, 0.f, -1.f}.Nomalize() * enemy_->DefaultSpeed);
	if (enemy_->GetWorldTransform().translation_.z <= 0.f) {
		enemy_->ChangeState(new EnemyState::Leave());
	}
}

void EnemyState::Approach::Exit() {}



void EnemyState::Leave::Enter() {}

void EnemyState::Leave::Update() {
	// 移動
	enemy_->AddPosition(
	    (enemy_->GetWorldTransform().translation_ - Vector3::zero()).Nomalize() *
	    enemy_->DefaultSpeed);
}

void EnemyState::Leave::Exit() {}
