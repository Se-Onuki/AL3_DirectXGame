#include "Entity.h"

void Entity::Init(const std::string& modelKey, const uint32_t& textureHandle) {
	ModelHash modelHash = std::hash<std::string>{}(modelKey);
	this->Init(ModelManager::GetInstance()->GetModel(modelHash), textureHandle);
}

void Entity::Init(Model* model, const uint32_t& textureHandle) {
	worldTransform_.Initialize();
	model_ = model;
	textureHandle_ = textureHandle;
}
