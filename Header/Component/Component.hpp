#pragma once
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Sprite.h"

#include "Vector2.h"
#include <memory>

#include <string>
#include <unordered_map>


class ModelManager {
public:
	static ModelManager& getInstance() {
		static ModelManager instance;
		return instance;
	}

	~ModelManager() {
		for (auto& pair : models) {
			delete pair.second;
		}
	}

	Model* getModel(const std::string& key) {
		auto it = models.find(key);
		if (it != models.end()) {
			return it->second;
		}
		return nullptr;
	}

	void addModel(const std::string& key, Model* model) { models[key] = model; }

private:
	ModelManager() {}
	std::unordered_map<std::string, Model*> models;
};

class SpriteManager {
public:
	static SpriteManager& getInstance() {
		static SpriteManager instance;
		return instance;
	}

	~SpriteManager() {
		for (auto& pair : sprites) {
			delete pair.second;
		}
	}

	Sprite* getSprite(const std::string& key) {
		auto it = sprites.find(key);
		if (it != sprites.end()) {
			return it->second;
		}
		return nullptr;
	}

	void addSprite(const std::string& key, Sprite* sprite) { sprites[key] = sprite; }

private:
	SpriteManager() {}
	std::unordered_map<std::string, Sprite*> sprites;
};

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
	~SpriteComp() { 
	}
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