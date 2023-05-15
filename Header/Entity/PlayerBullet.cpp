#include "PlayerBullet.h"
#include "Header/ModelManager/ModelManager.h"

PlayerBullet::PlayerBullet() {}

PlayerBullet::~PlayerBullet() {}

void PlayerBullet::Init(Model* model, const Vector3& position, const Vector3& velocity) {
	Entity::Init(model, TextureManager::Load("white1x1.png"));
	worldTransform_.translation_ = position;
	velocity_ = velocity;
}

void PlayerBullet::Update() {
	worldTransform_.translation_ += velocity_;
	worldTransform_.UpdateMatrix();
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}
