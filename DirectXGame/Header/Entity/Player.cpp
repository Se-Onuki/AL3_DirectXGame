#include "Player.h"
#include "Header/ModelManager/ModelManager.h"
#include <algorithm>
#include <imgui.h>

#include "ViewProjection.h"
#include "math/Math.hpp"

void Player::InitFloatingGimmick() { floatingParameter_ = 0.f; }

void Player::UpdateFloatingGimmick() {

	static int32_t cycle = 120u;
	const float step = Angle::PI_2 / cycle;

	static float cycleRange = 0.2f;
	static int32_t swayHand_Dig = 30;

	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head Transform", &worldTransformHead_.translation_.x, -10.f, 10.f);
	ImGui::SliderFloat3("Left Transform", &worldTransformLeft_.translation_.x, -10.f, 10.f);
	ImGui::SliderFloat3("Right Transform", &worldTransformRight_.translation_.x, -10.f, 10.f);
	ImGui::SliderInt("Cycle", &cycle, 1, 360);
	ImGui::SliderFloat("CycleRange", &cycleRange, 0.f, 3.f);
	ImGui::SliderInt("SwayHand", &swayHand_Dig, 0, 360);
	ImGui::End();

	floatingParameter_ += step;
	floatingParameter_ = std::fmod(floatingParameter_, Angle::PI_2);

	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * cycleRange + originPos;

	worldTransformRight_.rotation_.x = std::sin(floatingParameter_) * swayHand_Dig * Angle::Dig2Rad;
	worldTransformLeft_.rotation_.x = std::sin(floatingParameter_) * swayHand_Dig * Angle::Dig2Rad;
}

void Player::Init(
    Model* const modelBody, Model* const modelHead, Model* const modelL_arm,
    Model* const modelR_arm, const uint32_t& textureHandle) {

	// メモリ確保
	worldTransformOrigin_.Initialize();

	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformLeft_.Initialize();
	worldTransformRight_.Initialize();

	// 親子関係
	worldTransformBody_.parent_ = &worldTransformOrigin_;

	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformLeft_.parent_ = &worldTransformBody_;
	worldTransformRight_.parent_ = &worldTransformBody_;

	worldTransformHead_.translation_ = {0.f, 2.1f, 0.f};
	worldTransformRight_.translation_ = {+0.75f, 1.5f, 0.f};
	worldTransformLeft_.translation_ = {-0.75f, 1.5f, 0.f};

	modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelL_arm_ = modelL_arm;
	modelR_arm_ = modelR_arm;

	textureHandle_ = textureHandle;

	input_ = Input::GetInstance();
	InitFloatingGimmick();
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

			worldTransformOrigin_.translation_ += move; // 移動量を追加

			worldTransformOrigin_.rotation_ =
			    move.Direction2Euler(); // ベクトルからオイラー角を算出
		}
	}

	UpdateFloatingGimmick();

	worldTransformOrigin_.UpdateMatrix();

	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformRight_.UpdateMatrix();
	worldTransformLeft_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& Vp) const {
	modelBody_->Draw(worldTransformBody_, Vp);
	modelHead_->Draw(worldTransformHead_, Vp);
	modelR_arm_->Draw(worldTransformRight_, Vp);
	modelL_arm_->Draw(worldTransformLeft_, Vp);
}

Player::Player() { input_ = Input::GetInstance(); }

Player::~Player() {}
