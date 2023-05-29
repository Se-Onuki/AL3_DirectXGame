#include "RailCamera.h"
#include <imgui.h>

void RailCamera::Init(const Vector3& position, const Vector3& rotate) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotate;
	viewProjection_.Initialize();
}

void RailCamera::Update() {
	worldTransform_.translation_ += Vector3{0, 0, -0.1f};
	worldTransform_.rotation_ += Vector3::zero();
	worldTransform_.UpdateMatrix();

	viewProjection_.matView = worldTransform_.matWorld_.Inverse();

	ImGui::Begin("Camera");
	ImGui::DragFloat3("CameraPos", &worldTransform_.translation_.x);
	ImGui::DragFloat3("CameraRot", &worldTransform_.rotation_.x);
	ImGui::End();
}
