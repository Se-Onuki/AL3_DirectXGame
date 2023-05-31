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

	ImGui::Begin("Player");
	ImGui::Text("Position( %+.2f, %+.2f, %+.2f)", translate.x, translate.y, translate.z);
	ImGui::End();

#pragma region Mouse

	POINT mousePosition;
	GetCursorPos(&mousePosition);

	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

#pragma endregion

#pragma region Target2D

	sprite2DReticle_->SetPosition(
	    {static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)});

	// Vector3 positionReticle = worldTransform3DReticle_.translation_;
	// Matrix4x4 matViewport =
	//     Render::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// Matrix4x4 matViewProjectionViewport = Vp.matView * Vp.matProjection * matViewport;
	// positionReticle = positionReticle * matViewProjectionViewport;
	// sprite2DReticle_->SetPosition({positionReticle.x, positionReticle.y});

#pragma endregion

#pragma region 2D->3D

	Matrix4x4 matViewport =
	    Render::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matVPV = Vp.matView * Vp.matProjection * matViewport;
	Matrix4x4 matInvarseVPV = matVPV.Inverse();

	Vector3 posNear{static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y), 0.f};
	Vector3 posFar{static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y), 1.f};

	posNear = posNear * matInvarseVPV;
	posFar = posFar * matInvarseVPV;

#pragma endregion

#pragma region segment->Vector3

	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = mouseDirection.Nomalize();
	const float kDistanceTestObject = 100.f;
	worldTransform3DReticle_.translation_ =
	    mouseDirection.Nomalize() * kDistanceTestObject + posNear;
	worldTransform3DReticle_.UpdateMatrix();

#pragma endregion

	// const float kDistancePlayerTo3DReticle = 50.f;
	// Vector3 offset{0.f, 0.f, 1.f};
	// offset = TransformNormal(offset, GetWorldTransform().matWorld_);
	// offset = offset.Nomalize() * kDistancePlayerTo3DReticle;

	// worldTransform3DReticle_.translation_ = offset + GetPosition();
	// worldTransform3DReticle_.UpdateMatrix();

#pragma region 攻撃処理

	Attack();

#pragma endregion

	ImGui::Begin("Player");
	ImGui::Text(
	    "2DReticle:( %.2f, %.2f)", sprite2DReticle_->GetPosition().x,
	    sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near:( %+.2f, %+.2f, %+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:( %+.2f, %+.2f, %+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text(
	    "3DReticle:( %+.2f, %+.2f, %+.2f)", worldTransform3DReticle_.translation_.x,
	    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();
}

void Player::Attack() {
	if (input_->IsTriggerMouse(0)) {

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
	model_->Draw(worldTransform3DReticle_, Vp, textureHandle_);
	/*for (auto& element : *bullets_) {
	    element->Draw(Vp);
	}*/
}

void Player::DrawUI() const { sprite2DReticle_->Draw(); }

Player::Player() {}

Player::~Player() { delete sprite2DReticle_; }
