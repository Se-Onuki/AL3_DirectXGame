#include"Entity.hpp"

#include "Model.h"

#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Header/Component/Component.hpp"

Entity::Entity() {}

Entity::~Entity() {
	for (auto& element : componentList_) {
		delete element.second;
	}
	componentList_.clear();
}

void Entity::Init(Model* model, uint32_t textureHandle) {
	componentList_["transform"] = new TransformComp{};
	componentList_["model"] = new ModelComp{model};
	componentList_["textureHandle"] = new TextureComp{textureHandle};
	((TransformComp*)(componentList_["transform"]))->wTransform_.Initialize();
}

void Entity::Update() { 
	auto& wTransform = ((TransformComp*)(componentList_["transform"]))->wTransform_;
	wTransform.translation_.x += 1;
	wTransform.TransferMatrix();
}

void Entity::Draw(const ViewProjection& vProjection) {
	const auto model = ((ModelComp*)(componentList_["model"]))->model_;
	const auto& wTransform = ((TransformComp*)(componentList_["transform"]))->wTransform_;
	const auto texture = ((TextureComp*)(componentList_["textureHandle"]))->texture_;
	
	model->Draw(wTransform, vProjection, texture);
}
