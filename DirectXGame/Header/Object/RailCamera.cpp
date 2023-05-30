#include "RailCamera.h"
#include "Header/Object/Rail.h"
#include <imgui.h>

void RailCamera::Init(const Vector3& position, const Vector3& rotate) {
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotate;
	viewProjection_.Initialize();
	Direction_ = Vector3::zero();
}

void RailCamera::Update() {
	rail_->Update();
	const Ray& railPath = rail_->GetRailRay();

	viewProjection_.translation_ = railPath.origin;

	Direction_ = Slerp(Direction_, railPath.diff, 0.1f);
	viewProjection_.rotation_ = CalcFacing(Direction_);
	viewProjection_.UpdateViewMatrix();

	worldTransform_.matWorld_ = viewProjection_.matView.Inverse();

	// worldTransform_.translation_ += Vector3::zero();
	// worldTransform_.rotation_ += Vector3::zero();

	// worldTransform_.CalcMatrix();

	// viewProjection_.matView = worldTransform_.matWorld_.Inverse();

	ImGui::Begin("Camera");
	ImGui::DragFloat3("CameraPos", &viewProjection_.translation_.x);
	ImGui::DragFloat3("CameraRot", &viewProjection_.rotation_.x);
	ImGui::End();
}
