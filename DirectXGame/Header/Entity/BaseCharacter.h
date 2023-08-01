#pragma once
#include "WorldTransform.h"
#include <string>
#include <unordered_map>

class Model;
struct ViewProjection;

class BaseCharacter {

protected:
	std::unordered_map<std::string, Model*> modelMap_;
	WorldTransform transformOrigin_;

public:
	BaseCharacter() = default;
	virtual ~BaseCharacter() = default;

	virtual void Init(const std::unordered_map<std::string, Model*>& model);

	virtual void Update();

	virtual void Draw(const ViewProjection& vp) const;

	const WorldTransform& GetWorldTransform() const { return transformOrigin_; }

private:
};