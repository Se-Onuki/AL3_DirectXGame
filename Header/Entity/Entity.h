#pragma once
#include <WorldTransform.h>
#include "Header/ModelManager/ModelManager.h"

class Entity {
protected:
	WorldTransform worldTransform_;
	modelHash model_ = {};
	uint32_t textureHandle_ = 0u;

public:

	virtual void Draw(const ViewProjection& Vp) {
		ModelManager::GetInstance()->GetModel(model_)->Draw(worldTransform_, Vp, textureHandle_);
	}

	virtual void Init(const std::string& modelKey, const uint32_t& textureHandle);

	virtual void Update();

	virtual ~Entity() {}

};