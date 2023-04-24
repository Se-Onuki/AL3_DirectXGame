#pragma once
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Sprite.h"

#include "Vector2.h"

class ComponentData {};

class TransformComp : ComponentData {
public:
	WorldTransform wTransform_;
};

class ModelComp : ComponentData {
public:
	ModelComp(Model* model) : model_(model) {}
	Model* model_;
};

class TextureComp : ComponentData {
public:
	TextureComp(uint32_t texHandle) : texture_(texHandle) {}
	uint32_t texture_;
};

class SpriteComp : ComponentData {
public:
	SpriteComp() {}
	SpriteComp(Sprite* spritePtr) : sprite_(spritePtr) {}
	~SpriteComp() { delete sprite_; }
	Sprite* sprite_ = nullptr;
};

class PositionComp : ComponentData {
public:
	Vector2 poistion_;
};

class VelocityComp : ComponentData {
public:
	Vector2 velocity_;
};