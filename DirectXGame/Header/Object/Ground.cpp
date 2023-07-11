#include "Ground.h"
#include "Header/ModelManager/ModelManager.h"

Ground::Ground() {}

Ground::~Ground() {}

void Ground::Init() {
	model_ = ModelManager::GetInstance()->GetModel("Ground");
	worldTransform_.Initialize();
	worldTransform_.scale_ *= 500.f;
}

void Ground::Update() { worldTransform_.UpdateMatrix(); }

void Ground::Draw(const ViewProjection& Vp) const { model_->Draw(worldTransform_, Vp); }