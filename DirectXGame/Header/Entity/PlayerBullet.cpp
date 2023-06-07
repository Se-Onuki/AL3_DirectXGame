#include "PlayerBullet.h"
#include "Header/ModelManager/ModelManager.h"

#include "Header/Entity/Enemy.h"
#include "Lerp.h"

PlayerBullet::PlayerBullet() {}

PlayerBullet::~PlayerBullet() {}

void PlayerBullet::OnCollision(const Collider* const) { isDead_ = true; }

void PlayerBullet::Init(Model* model, const Vector3& position, const Vector3& velocity) {
	Entity::Init(model, TextureManager::Load("white1x1.png"));
	worldTransform_.translation_ = position;
	velocity_ = velocity;
	SetCollisionAttribute(static_cast<uint32_t>(CollisionFilter::Player));
	SetCollisionMask(~(static_cast<uint32_t>(CollisionFilter::Player)));
}

void PlayerBullet::Update() {
	Move();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::SetFacing() {
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	worldTransform_.rotation_.x =
	    std::atan2(-velocity_.y, std::sqrt(std::powf(velocity_.x, 2) + std::powf(velocity_.z, 2)));
}

std::list<Collider*> HomingPlayerBullet::targetList_ = {};

void HomingPlayerBullet::SetTarget(Collider* const enemy) {
	target_ = enemy;
	auto element = std::find(targetList_.begin(), targetList_.end(), target_);
	if (element == targetList_.end()) {
		targetList_.emplace_back(target_);
	}
}

void HomingPlayerBullet::OnCollision(const Collider* const other) {
	std::erase(targetList_, other);
	PlayerBullet::OnCollision(other);
}

void HomingPlayerBullet::Update() {
	if (target_) {
		auto element = std::find(targetList_.begin(), targetList_.end(), target_);
		if (element != targetList_.end()) {
			velocity_ =
			    Slerp(velocity_, (target_->GetPosition() - GetPosition()), 0.125f).Nomalize() *
			    kBulletSpeed;
		}
	}
	SetFacing();
	PlayerBullet::Update();
}
