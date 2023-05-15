#include "Player.h"
#include "Header/ModelManager/ModelManager.h"

void Player::Init(const std::string& modelKey, const uint32_t& textureHandle) {
	worldTransform_.Initialize();
	model_ = std::hash<std::string>{}(modelKey);
	textureHandle_ = textureHandle;
}

void Player::Update() { worldTransform_.TransferMatrix(); }
