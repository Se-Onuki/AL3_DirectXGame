#pragma once
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"

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