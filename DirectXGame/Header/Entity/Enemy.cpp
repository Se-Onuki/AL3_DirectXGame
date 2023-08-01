#include "Enemy.h"
#include "Model.h"
#include <Math.hpp>
#include <imgui.h>

Enemy::~Enemy() {}

void Enemy::Init(const std::unordered_map<std::string, Model*>& model) {
	BaseCharacter::Init(model);
}

void Enemy::Update() {
	static float rotateAngle = 2.f * Angle::Dig2Rad;
	static float moveSpeed = 0.3f;
	ImGui::Begin("Enemy");
	ImGui::SliderAngle("RotateY", &rotateAngle);
	ImGui::SliderFloat("MoveSpeed", &moveSpeed, 0.f, 10.f);
	ImGui::End();

	transformOrigin_.translation_ +=
	    TransformNormal(Vector3::front() * moveSpeed, transformOrigin_.matWorld_);
	transformOrigin_.rotation_.y += rotateAngle;
	BaseCharacter::Update();
}

void Enemy::Draw(const ViewProjection& Vp) const {
	modelMap_.at("body")->Draw(transformOrigin_, Vp);
}
