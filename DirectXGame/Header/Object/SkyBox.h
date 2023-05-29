#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class Model;

class SkyBox {
public:
	void Init();
	void Update();
	void Draw(const ViewProjection& Vp) const;

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
};
