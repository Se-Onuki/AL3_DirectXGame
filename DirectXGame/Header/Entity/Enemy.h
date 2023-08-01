#pragma once

#include "BaseCharacter.h"

class Enemy : public BaseCharacter {
public:
	Enemy() = default;
	~Enemy() override;

	void Init(const std::unordered_map<std::string, Model*>& model) override;
	void Update() override;
	void Draw(const ViewProjection& Vp) const override;

private:
};
