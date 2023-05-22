#include "Enemy.h"
#include "math/Math.hpp"
#include <imgui.h>

const float Enemy::DefaultSpeed = 0.2f;

void Enemy::ChangeState(EnemyState::Base* newState) {
	if (state_)
		state_->Exit();
	delete state_;
	state_ = newState;
	state_->enemy_ = this;
	state_->Enter();
}

void Enemy::Fire() {

	Vector3 velocity(0, 0, kBulletSpeed);
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Init(model_, worldTransform_.translation_, velocity);

	bullets_.emplace_back(newBullet);
}

Enemy::Enemy() { ChangeState(new EnemyState::Approach()); }

Enemy::~Enemy() {}

void Enemy::Init(Model* model, const uint32_t& textureHandle, const Vector3& position) {
	Entity::Init(model, textureHandle);
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = Angle::Digree(180).ToRadian();
	worldTransform_.UpdateMatrix();
	cooltime_ = maxCooltime_;
}

void Enemy::Update() {
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->IsDead()) {
			bullet.reset();
			return true;
		}
		return false;
	});
	state_->Update();
	for (auto& bullet : bullets_) {
		bullet->Update();
	}
	worldTransform_.UpdateMatrix();
	const Vector3& translation = worldTransform_.translation_;
	ImGui::Begin("Enemy");
	ImGui::Text("x: %.2f, y: %.2f, z: %.2f", translation.x, translation.y, translation.z);
	ImGui::End();
}

void Enemy::Draw(const ViewProjection& Vp) {
	Entity::Draw(Vp);
	for (auto& bullet : bullets_) {
		bullet->Draw(Vp);
	}
}

void EnemyState::Approach::Enter() { enemy_->ResetCooltime(); }

void EnemyState::Approach::Update() {
	// 移動
	enemy_->AddPosition(Vector3{0.f, 0.f, -1.f}.Nomalize() * enemy_->DefaultSpeed);
	if (enemy_->GetWorldTransform().translation_.z <= 0.f) {
		enemy_->ChangeState(new EnemyState::Leave());
		return;
	}
	enemy_->FireTimer();
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

void EnemyBullet::Init(Model* model, const Vector3& position, const Vector3& velocity) {
	Entity::Init(model, TextureManager::Load("white1x1.png"));
	worldTransform_.translation_ = position;
	velocity_ = velocity;
}

void EnemyBullet::Update() {
	Move();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}