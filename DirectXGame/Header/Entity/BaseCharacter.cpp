#include "BaseCharacter.h"

#include "Model.h"
#include "ViewProjection.h"

void BaseCharacter::Init(const std::unordered_map<std::string, Model*>& model) {
	modelMap_ = model;
	transformOrigin_.Initialize();
}

void BaseCharacter::Update() { transformOrigin_.UpdateMatrix(); }

void BaseCharacter::Draw(const ViewProjection& vp) const {
	for (auto& model : modelMap_) {
		model.second->Draw(transformOrigin_, vp);
	}
}
