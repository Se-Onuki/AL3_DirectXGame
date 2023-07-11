#pragma once
#include <ViewProjection.h>
#include <WorldTransform.h>

class Model;

class Ground {
	Model* model_ = nullptr;
	WorldTransform worldTransform_;

public:
	Ground();
	~Ground();

	void Init();
	void Update();

	void Draw(const ViewProjection& Vp) const;
};
