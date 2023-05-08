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

struct Identifier : ComponentData {
	char name[10];
	void Init(const char * value) { 
		for (uint32_t i = 0; i < 10; i++) {
			name[i] = value[i];
		}
	}
};

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
	void Init(const std::string& key, Sprite* value) {
		sprite_ = std::hash<std::string>{}(key);
		SpriteManager::GetInstance()->AddSprite(sprite_, value);
	}
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

struct InputFlagComp : ComponentData {};