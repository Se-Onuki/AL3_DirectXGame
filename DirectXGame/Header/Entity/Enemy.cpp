#include "Enemy.h"
#include "math/Math.hpp"
#include <imgui.h>

#include "GameScene.h"
#include "Player.h"
#include "math/Lerp.h"

const float Enemy::DefaultSpeed = 0.1f;

void Enemy::ChangeState(EnemyState::Base* newState) {
	if (state_)
		state_->Exit();
	delete state_;
	state_ = newState;
	state_->enemy_ = this;
	state_->Enter();
}

void Enemy::SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

void Enemy::OnCollision(const Collider* const) { isDead_ = true; }

void Enemy::Fire() {

	assert(player_);
	assert(model_);

	Vector3 velocity = (player_->GetPosition() - GetPosition()).Nomalize() * kBulletSpeed;

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Init(model_, worldTransform_.translation_, velocity);
	newBullet->SetPlayer(player_);

	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::FireAndInit() {
	Fire();
	SetFireTimer();
}

void Enemy::SetFireTimer() {
	fireTimerList_.emplace_back(
	    new FunctionTimer(std::bind(&Enemy::FireAndInit, this), this->maxCooltime_));
}

void Enemy::FireTimerDelete() { fireTimerList_.clear(); }

Enemy::Enemy() { ChangeState(new EnemyState::Approach()); }

Enemy::~Enemy() {}

void Enemy::Init(Model* model, const uint32_t& textureHandle, const Vector3& position) {
	Entity::Init(model, textureHandle);
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = Angle::Digree(180).ToRadian();
	worldTransform_.UpdateMatrix();

	SetCollisionAttribute(static_cast<uint32_t>(CollisionFilter::Enemy));
	SetCollisionMask(~(static_cast<uint32_t>(CollisionFilter::Enemy)));
}

void Enemy::Update() {

	fireTimerList_.remove_if([](std::unique_ptr<FunctionTimer>& fireTimer) {
		if (fireTimer->IsFinish()) {
			fireTimer.reset();
			return true;
		}
		return false;
	});

	for (auto& fireTimer : fireTimerList_) {
		fireTimer->Update();
	}

	state_->Update();
	/*for (auto& bullet : bullets_) {
	    bullet->Update();
	}*/
	worldTransform_.UpdateMatrix();
	const Vector3& translation = worldTransform_.translation_;
	ImGui::Begin("Enemy");
	ImGui::Text("x: %.2f, y: %.2f, z: %.2f", translation.x, translation.y, translation.z);
	ImGui::Text(
	    "%f", std::acos(Vector3{0, 0, -1} * (player_->GetPosition() - translation).Nomalize()));
	ImGui::End();
}

void Enemy::Draw(const ViewProjection& Vp) {
	Entity::Draw(Vp);
	/*for (auto& bullet : bullets_) {
	    bullet->Draw(Vp);
	}*/
}

void EnemyState::Approach::Enter() { enemy_->SetFireTimer(); }

void EnemyState::Approach::Update() {
	// 移動
	enemy_->AddPosition(Vector3{0.f, 0.f, -0.f}.Nomalize() * enemy_->DefaultSpeed);
	if (enemy_->GetWorldTransform().translation_.z <= 0.f) {
		enemy_->ChangeState(new EnemyState::Leave());
		return;
	}
}

void EnemyState::Approach::Exit() { enemy_->FireTimerDelete(); }

void EnemyState::Leave::Enter() {}

void EnemyState::Leave::Update() {
	// 移動
	enemy_->AddPosition(
	    (enemy_->GetWorldTransform().translation_ - Vector3::zero()).Nomalize() *
	    enemy_->DefaultSpeed);
}

void EnemyState::Leave::Exit() {}

void EnemyBullet::OnCollision(const Collider* const) { isDead_ = true; }

void EnemyBullet::SetPlayer(Player* player) { player_ = player; }

void EnemyBullet::SetFacing() {
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	worldTransform_.rotation_.x =
	    std::atan2(-velocity_.y, std::sqrt(std::powf(velocity_.x, 2) + std::powf(velocity_.z, 2)));
}

void EnemyBullet::Init(Model* model, const Vector3& position, const Vector3& velocity) {
	Entity::Init(model, TextureManager::Load("white1x1.png"));
	worldTransform_.translation_ = position;
	velocity_ = velocity;
	worldTransform_.scale_ = {0.5f, 0.5f, 3.f};
	SetFacing();

	SetCollisionAttribute(static_cast<uint32_t>(CollisionFilter::Enemy));
	SetCollisionMask(~(static_cast<uint32_t>(CollisionFilter::Enemy)));
}

void EnemyBullet::Update() {
	velocity_ = Slerp(velocity_, (player_->GetPosition() - worldTransform_.translation_), 0.0125f)
	                .Nomalize() *
	            kBulletSpeed;
	SetFacing();
	Move();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}