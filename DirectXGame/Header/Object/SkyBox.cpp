#include "SkyBox.h"
#include "Header/ModelManager/ModelManager.h"
#include "Model.h"

void SkyBox::Init() {
	model_ = ModelManager::GetInstance()->GetModel("SkyBox");
	worldTransform_.Initialize();
	worldTransform_.scale_ *= 100;
}

void SkyBox::Update() { worldTransform_.UpdateMatrix(); }

void SkyBox::Draw(const ViewProjection& Vp) const { model_->Draw(worldTransform_, Vp); }
