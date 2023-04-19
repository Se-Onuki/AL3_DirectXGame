#pragma once
#include "Vector3.h"
#include "Model.h"
#include "WorldTransform.h"

struct IComponentData {};

struct TransformComp : IComponentData {
	WorldTransform wTransform_;
};

struct ModelComp : IComponentData {
	ModelComp(Model* model):model_(model){}
	Model* model_;
};

struct TextureComp : IComponentData {
	TextureComp(uint32_t texHandle) : texture_(texHandle) {}
	uint32_t texture_;
};/*

class Component {
public:
	Component();
	~Component();

private:
};


class Transform :public Component{
public:
	Transform();
	~Transform();

	Vector3 position;
	Vector3 scale;
	Vector3 rotate;

private:
};*/