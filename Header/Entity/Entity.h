#pragma once
#include "Header/ModelManager/ModelManager.h"
#include <WorldTransform.h>

class Entity {
protected:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Vector3 velocity_ = {};

public:
	virtual ~Entity() {}

	virtual void Draw(const ViewProjection& Vp) {
		model_->Draw(worldTransform_, Vp, textureHandle_);
	}

	virtual void Init(const std::string& modelKey, const uint32_t& textureHandle);
	virtual void Init(Model* model, const uint32_t& textureHandle);

	virtual void Update() {}

	void Move() {
		worldTransform_.translation_ += velocity_;
		worldTransform_.UpdateMatrix();
	}
};