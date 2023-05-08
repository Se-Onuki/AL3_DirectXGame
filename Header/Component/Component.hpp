#pragma once
#include "Model.h"
#include "Sprite.h"
#include "Vector3.h"
#include "WorldTransform.h"

#include "Vector2.h"
#include <memory>

#include <string>
#include <unordered_map>

#include "Header/ModelManager/ModelManager.hpp"

// class SpriteManager {
// public:
//	static SpriteManager& getInstance() {
//		static SpriteManager instance;
//		return instance;
//	}
//
//	~SpriteManager() {
//		for (auto& pair : sprites) {
//			delete pair.second;
//		}
//	}
//
//	Sprite* getSprite(const std::string& key) {
//		auto it = sprites.find(key);
//		if (it != sprites.end()) {
//			return it->second;
//		}
//		return nullptr;
//	}
//
//	void addSprite(const std::string& key, Sprite* sprite) { sprites[key] = sprite; }
//
// private:
//	SpriteManager() {}
//	std::unordered_map<std::string, Sprite*> sprites;
// };

struct ComponentData {};

struct TransformComp : ComponentData {
	WorldTransform wTransform_;
};

struct ModelComp : ComponentData {
	void Init(const std::string& key, Model* value) {
		model_ = std::hash<std::string>{}(key);
		ModelManager::GetInstance()->AddModel(model_, value);
	}
	modelHash model_;
};

struct SpriteComp : ComponentData {
	SpriteComp() {}
	SpriteComp(const std::string& key, Sprite* value) : sprite_(std::hash<std::string>{}(key)) {}
	spriteHash sprite_;
};

struct TextureComp : ComponentData {
	uint32_t texture_;
};

struct PositionComp : ComponentData {
	Vector3 poistion_;
};

struct VelocityComp : ComponentData {
	Vector3 velocity_;
};