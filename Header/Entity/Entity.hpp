#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Model.h"

#include "ViewProjection.h"
#include "Header/Component/Component.hpp"

//struct IComponentData;

class Entity {
public:
	Entity();
	 ~Entity();

	std::unordered_map<std::string, IComponentData*> componentList_;

	 void Init(Model* model_, uint32_t textureHandle);

	 void Update();
	 void Draw(const ViewProjection& vProjection);

private:
};

//
// class Entity {
// public:
//	Entity(int32_t id) : id(id) {}
//	~Entity();
//	const int32_t id;
//
// private:
//};