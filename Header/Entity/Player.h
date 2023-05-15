#pragma once
#include "Header/Entity/Entity.h"

class Player : public Entity {

public:
	void Init(const std::string& modelKey, const uint32_t& textureHandle) override;
	void Update() override;
};