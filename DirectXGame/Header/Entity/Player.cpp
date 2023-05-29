#include "Player.h"
#include "Header/ModelManager/ModelManager.h"
#include <algorithm>
#include <imgui.h>

#include "PlayerBullet.h"
#include "math/Math.hpp"

void Player::Init(Model* model, const uint32_t& textureHandle) {
	Entity::Init(model, textureHandle);
	input_ = Input::GetInstance();
	SetCollisionAttribute(static_cast<uint32_t>(CollisionFilter::Player));
	SetCollisionMask(~(static_cast<uint32_t>(CollisionFilter::Player)));
}

void Player::OnCollision() {}

void Player::Update() {

	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (bullet->IsDead()) {
			bullet.reset();
			return true;
		}
		return false;
	});

#pragma region 旋回処理

	Vector3 rotateDirection = {};

	if (input_->PushKey(DIK_E)) {
		rotateDirection.y += kRotSpeed;
	}
	if (input_->PushKey(DIK_Q)) {
		rotateDirection.y -= kRotSpeed;
	}

#pragma endregion

#pragma region 移動処理

	Vector3 moveDirection = {};

#pragma region 入力方向取得

	if (input_->PushKey(DIK_A)) {
		moveDirection.x -= 1;
	}
	if (input_->PushKey(DIK_D)) {
		moveDirection.x += 1;
	}

	if (input_->PushKey(DIK_W)) {
		moveDirection.y += 1;
	}
	if (input_->PushKey(DIK_S)) {
		moveDirection.y -= 1;
	}

#pragma endregion

	moveDirection = moveDirection.Nomalize() * MoveSpeed;

#pragma endregion

	// Vector3& scale = worldTransform_.scale_;
	Vector3& rotate = worldTransform_.rotation_;
	Vector3& translate = worldTransform_.translation_;

	rotate += rotateDirection;

	translate += moveDirection;
	translate.x = std::clamp(translate.x, -MovementLimit.x, MovementLimit.x);
	translate.y = std::clamp(translate.y, -MovementLimit.y, MovementLimit.y);

	worldTransform_.UpdateMatrix();

	ImGui::Begin("playerPosition");
	ImGui::Text("x: %.2f, y: %.2f, z: %.2f", translate.x, translate.y, translate.z);
	ImGui::End();

#pragma region 攻撃処理

	Attack();

#pragma endregion

	for (auto& element : bullets_) {
		element->Update();
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		Vector3 velocity(0, 0, kBulletSpeed);
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Init(model_, GetPosition(), velocity);

		bullets_.emplace_back(newBullet);
	}
}

void Player::Draw(const ViewProjection& Vp) {
	Entity::Draw(Vp);
	for (auto& element : bullets_) {
		element->Draw(Vp);
	}
}

Player::Player() {}

Player::~Player() {}
