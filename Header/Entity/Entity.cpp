#include "Entity.h"

void Entity::Init(const std::string& modelKey, const uint32_t& textureHandle) {
	worldTransform_.Initialize();
	model_ = std::hash<std::string>{}(modelKey);
	textureHandle_ = textureHandle;
}

void Entity::Update() {}
