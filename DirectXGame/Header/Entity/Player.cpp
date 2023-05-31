#include "Player.h"
#include "Header/ModelManager/ModelManager.h"
#include <algorithm>
#include <imgui.h>

#include "GameScene.h"
#include "Header/Render/Render.hpp"
#include "PlayerBullet.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "math/Math.hpp"

void Player::SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

void Player::Init(Model* model, const uint32_t& textureHandle) {
	Entity::Init(model, textureHandle);
	input_ = Input::GetInstance();
	SetCollisionAttribute(static_cast<uint32_t>(CollisionFilter::Player));
	SetCollisionMask(~(static_cast<uint32_t>(CollisionFilter::Player)));
	worldTransform3DReticle_.Initialize();
	uint32_t textureReticle = TextureManager::Load("target.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, {0, 0}, {1, 1, 1, 1}, {0.5f, 0.5f});
}

void Player::OnCollision() {}

void Player::Update(const ViewProjection& Vp) {

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

	const float kDistancePlayerTo3DReticle = 50.f;
	Vector3 offset{0.f, 0.f, 1.f};
	offset = TransformNormal(offset, GetWorldTransform().matWorld_);
	offset = offset.Nomalize() * kDistancePlayerTo3DReticle;

	worldTransform3DReticle_.translation_ = offset + GetPosition();
	worldTransform3DReticle_.UpdateMatrix();

#pragma region Target2D

	Vector3 positionReticle = worldTransform3DReticle_.translation_;
	Matrix4x4 matViewport =
	    Render::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matViewProjectionViewport = Vp.matView * Vp.matProjection * matViewport;
	positionReticle = positionReticle * matViewProjectionViewport;
	sprite2DReticle_->SetPosition({positionReticle.x, positionReticle.y});

#pragma endregion

#pragma region 攻撃処理

	Attack();

#pragma endregion
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		// Vector3 velocity(0, 0, kBulletSpeed);
		// velocity = TransformNormal(velocity, worldTransform_.matWorld_);
		Vector3 velocity = worldTransform3DReticle_.translation_ - GetPosition();
		velocity = velocity.Nomalize() * kBulletSpeed;

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Init(model_, GetPosition(), velocity);

		gameScene_->AddPlayerBullet(newBullet);
	}
}

void Player::Draw(const ViewProjection& Vp) {
	Entity::Draw(Vp);
	//model_->Draw(worldTransform3DReticle_, Vp, textureHandle_);
	/*for (auto& element : *bullets_) {
	    element->Draw(Vp);
	}*/
}

void Player::DrawUI() const { sprite2DReticle_->Draw(); }

Player::Player() {}

Player::~Player() { delete sprite2DReticle_; }
