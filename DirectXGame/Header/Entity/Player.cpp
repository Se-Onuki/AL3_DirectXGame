#include "Player.h"
#include "Header/ModelManager/ModelManager.h"
#include <algorithm>
#include <imgui.h>

#include "ViewProjection.h"
#include "math/Math.hpp"
#include <Header/File/GlobalVariables.h>
#include <Lerp.h>

void Player::ApplyClobalVariables() {
	GlobalVariables* const gVariables = GlobalVariables::GetInstance();
	gVariables;
	const char* groupName = "Player";
	groupName;
	worldTransformHead_.translation_ = gVariables->Get<Vector3>(groupName, "Head Translation");
	worldTransformLeft_.translation_ = gVariables->Get<Vector3>(groupName, "ArmL Translation");
	worldTransformRight_.translation_ = gVariables->Get<Vector3>(groupName, "ArmR Translation");

	floatingCycle_ = gVariables->Get<decltype(floatingCycle_)>(groupName, "floatingCycle");
	floatingSwayHand_ = gVariables->Get<decltype(floatingSwayHand_)>(groupName, "floatingSwayHand");
	floatingCycleRange_ =
	    gVariables->Get<decltype(floatingCycleRange_)>(groupName, "floatingCycleRange");

	attackClampAngle_ = gVariables->Get<decltype(attackClampAngle_)>(groupName, "attackClampAngle");
	attackCycle_ = gVariables->Get<decltype(attackCycle_)>(groupName, "attackCycle");
	attackStartAngle_ = gVariables->Get<decltype(attackStartAngle_)>(groupName, "attackStartAngle");
	attackSwingAngle_ = gVariables->Get<decltype(attackSwingAngle_)>(groupName, "attackSwingAngle");
}

void Player::InitFloatingGimmick() { floatingParameter_ = 0.f; }

void Player::UpdateFloatingGimmick() {

	const float step = Angle::PI_2 / floatingCycle_;

	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head Transform", &worldTransformHead_.translation_.x, -10.f, 10.f);
	ImGui::SliderFloat3("Left Transform", &worldTransformLeft_.translation_.x, -10.f, 10.f);
	ImGui::SliderFloat3("Right Transform", &worldTransformRight_.translation_.x, -10.f, 10.f);
	ImGui::SliderInt("Cycle", &floatingCycle_, 1, 360);
	ImGui::SliderFloat("CycleRange", &floatingCycleRange_, 0.f, 3.f);
	ImGui::SliderAngle("SwayHand", &floatingSwayHand_, 0.f, 360.f);
	ImGui::End();

	floatingParameter_ += step;
	floatingParameter_ = std::fmod(floatingParameter_, Angle::PI_2);

	worldTransformBody_.translation_.y =
	    std::sin(floatingParameter_) * floatingCycleRange_ + originPos;

	worldTransformRight_.rotation_.x = std::sin(floatingParameter_) * floatingSwayHand_;
	worldTransformLeft_.rotation_.x = std::sin(floatingParameter_) * floatingSwayHand_;
}

void Player::BehaviorRootInit() { floatingParameter_ = 0.f; }

void Player::BehaviorRootUpdate() {

	XINPUT_STATE inputState{};
	if (input_->GetJoystickState(0, inputState)) {

		// 左スティックのデータを受け取る
		Vector3 move{(float)inputState.Gamepad.sThumbLX, 0.f, (float)inputState.Gamepad.sThumbLY};
		if (move != Vector3::zero()) {

			move = move.Nomalize() * moveSpeed_; // 速度を正規化
			move =                               // カメラ方向に向けて回転
			    move *
			    Matrix4x4::EulerRotate(Matrix4x4::EulerAngle::Yaw, viewProjection_->rotation_.y);

			transformOrigin_.translation_ += move; // 移動量を追加

			transformOrigin_.rotation_ = move.Direction2Euler(); // ベクトルからオイラー角を算出
		}

		if (inputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			behaviorRequest_ = Behavior::kAttack;
		}
	}

	UpdateFloatingGimmick();
	UpdateWorldMatrix();
}

void Player::BehaviorAttackInit() { floatingParameter_ = 0.f; }

void Player::BehaviorAttackUpdate() {

	const float step = Angle::PI_2 / attackCycle_;

	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head Transform", &worldTransformHead_.translation_.x, -10.f, 10.f);
	ImGui::SliderFloat3("Left Transform", &worldTransformLeft_.translation_.x, -10.f, 10.f);
	ImGui::SliderFloat3("Right Transform", &worldTransformRight_.translation_.x, -10.f, 10.f);
	ImGui::SliderInt("Cycle", &attackCycle_, 1, 360);
	ImGui::SliderAngle("CycleRange", &attackStartAngle_);
	ImGui::SliderAngle("SwayHand", &attackSwingAngle_);
	ImGui::SliderAngle("ClampAngle", &attackClampAngle_);
	ImGui::End();

	floatingParameter_ += step;
	if (floatingParameter_ >= Angle::PI_2) {
		behaviorRequest_ = Behavior::kRoot;
	}
	floatingParameter_ = std::fmod(floatingParameter_, Angle::PI_2);

	worldTransformWeapon_.rotation_.x = std::clamp<float>(
	    std::sin(floatingParameter_) * attackSwingAngle_ + attackStartAngle_, 0.f,
	    attackClampAngle_);
	worldTransformLeft_.rotation_.x =
	    std::clamp<float>(
	        std::sin(floatingParameter_) * attackSwingAngle_ + attackStartAngle_, 0.f,
	        attackClampAngle_) +
	    Angle::PI;
	worldTransformRight_.rotation_.x =
	    std::clamp<float>(
	        std::sin(floatingParameter_) * attackSwingAngle_ + attackStartAngle_, 0.f,
	        attackClampAngle_) +
	    Angle::PI;

	UpdateWorldMatrix();
}

void Player::UpdateWorldMatrix() {
	transformOrigin_.UpdateMatrix();

	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformRight_.UpdateMatrix();
	worldTransformLeft_.UpdateMatrix();

	worldTransformWeapon_.UpdateMatrix();
}

void Player::Init(const std::unordered_map<std::string, Model*>& model) {
	GlobalVariables* const gVariables = GlobalVariables::GetInstance();
	const char* const groupName = "Player";
	ApplyClobalVariables();

	BaseCharacter::Init(model);

	// メモリ確保
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformLeft_.Initialize();
	worldTransformRight_.Initialize();

	worldTransformWeapon_.Initialize();

	// 親子関係
	worldTransformBody_.parent_ = &transformOrigin_;

	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformLeft_.parent_ = &worldTransformBody_;
	worldTransformRight_.parent_ = &worldTransformBody_;

	worldTransformWeapon_.parent_ = &worldTransformBody_;

	worldTransformHead_.translation_ = {0.f, 2.1f, 0.f};
	worldTransformRight_.translation_ = {+0.75f, 1.5f, 0.f};
	worldTransformLeft_.translation_ = {-0.75f, 1.5f, 0.f};

	worldTransformWeapon_.translation_ = {0.f, 2.f, 0.f};

	gVariables->AddValue(groupName, "Head Translation", worldTransformHead_.translation_);
	gVariables->AddValue(groupName, "ArmL Translation", worldTransformLeft_.translation_);
	gVariables->AddValue(groupName, "ArmR Translation", worldTransformRight_.translation_);

	gVariables->AddValue(groupName, "floatingCycle", floatingCycle_);
	gVariables->AddValue(groupName, "floatingSwayHand", floatingSwayHand_);
	gVariables->AddValue(groupName, "floatingCycleRange", floatingCycleRange_);

	gVariables->AddValue(groupName, "attackClampAngle", attackClampAngle_);
	gVariables->AddValue(groupName, "attackCycle", attackCycle_);
	gVariables->AddValue(groupName, "attackStartAngle", attackStartAngle_);
	gVariables->AddValue(groupName, "attackSwingAngle", attackSwingAngle_);

	input_ = Input::GetInstance();
	InitFloatingGimmick();
}

void Player::Update() {
	if (behaviorRequest_) {
		// 振舞いの更新
		behavior_ = behaviorRequest_.value();
		// 振舞いの初期化
		switch (behavior_) {
		case Player::Behavior::kRoot:
			BehaviorRootInit();
			break;
		case Player::Behavior::kAttack:
			BehaviorAttackInit();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
	case Player::Behavior::kRoot:
		BehaviorRootUpdate();
		break;
	case Player::Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	}
	if (input_->TriggerKey(DIK_R)) {
		ApplyClobalVariables();
	}
}

void Player::Draw(const ViewProjection& Vp) const {
	modelMap_.at("body")->Draw(worldTransformBody_, Vp);
	modelMap_.at("head")->Draw(worldTransformHead_, Vp);
	modelMap_.at("right")->Draw(worldTransformRight_, Vp);
	modelMap_.at("left")->Draw(worldTransformLeft_, Vp);

	if (behavior_ == Player::Behavior::kAttack)
		modelMap_.at("weapon")->Draw(worldTransformWeapon_, Vp);
}

Player::Player() { input_ = Input::GetInstance(); }

Player::~Player() {}
