#include "Player.h"
#include "Header/ModelManager/ModelManager.h"
#include <algorithm>
#include <imgui.h>

#include "math/Math.hpp"

void Player::Init(Model* model, const uint32_t& textureHandle) {
	Entity::Init(model, textureHandle);
	input_ = Input::GetInstance();
}

void Player::Update() {

	XINPUT_STATE inputState{};
	if (input_->GetJoystickState(0, inputState)) {

		// 左スティックのデータを受け取る
		Vector3 move{(float)inputState.Gamepad.sThumbLX, 0.f, (float)inputState.Gamepad.sThumbLY};
		if (move != Vector3::zero()) {

			move = move.Nomalize() * moveSpeed_; // 速度を正規化
			move =                               // カメラ方向に向けて回転
			    move *
			    Matrix4x4::EulerRotate(Matrix4x4::EulerAngle::Yaw, viewProjection_->rotation_.y);

			worldTransform_.translation_ += move; // 移動量を追加

			worldTransform_.rotation_ = move.Direction2Euler(); // ベクトルからオイラー角を算出
		}
	}

	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& Vp) { Entity::Draw(Vp); }

Player::Player() { input_ = Input::GetInstance(); }

Player::~Player() {}
