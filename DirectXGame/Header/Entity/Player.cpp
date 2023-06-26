#include "Player.h"
#include "Header/ModelManager/ModelManager.h"
#include <algorithm>
#include <imgui.h>

#include "math/Math.hpp"

void Player::Init(Model* model, const uint32_t& textureHandle) {
	Entity::Init(model, textureHandle);
	input_ = Input::GetInstance();
}

void Player::Update() {}

void Player::Draw(const ViewProjection& Vp) { Entity::Draw(Vp); }

Player::Player() {}

Player::~Player() {}
