#include "PlayerBullet.h"
#include "Header/ModelManager/ModelManager.h"

PlayerBullet::PlayerBullet() {}

PlayerBullet::~PlayerBullet() {}

void PlayerBullet::OnCollision() { isDead_ = true; }

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
