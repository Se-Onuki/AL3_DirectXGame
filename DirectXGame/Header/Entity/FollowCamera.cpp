#include "FollowCamera.h"
#include "Input.h"
#include "WorldTransform.h"

void FollowCamera::Init() {
	viewProjection_.Initialize();
	input_ = Input::GetInstance();
}

void FollowCamera::Update() {
	if (target_) {
		XINPUT_STATE inputState;
		Vector3 offset{0.f, 3.f, -15.f};

		if (input_->GetJoystickState(0, inputState)) {
			const float rotateStick = inputState.Gamepad.sThumbRX / 32767.f;
			rotate_.y += rotateStick * cameraRotSpeed_;

			const Matrix4x4& mat = Matrix4x4::EulerRotate(Matrix4x4::Yaw, rotate_.y);

			offset = TransformNormal(offset, mat);

			viewProjection_.rotation_.y = rotate_.y;
		}

		viewProjection_.translation_ = target_->translation_ + offset;
	}
	viewProjection_.UpdateMatrix();
}

const Matrix4x4& FollowCamera::GetViewMatrix() const { return viewProjection_.matView; }

const Matrix4x4& FollowCamera::GetProjectionMatrix() const { return viewProjection_.matProjection; }

const ViewProjection* const FollowCamera::GetViewProjection() const { return &viewProjection_; }
