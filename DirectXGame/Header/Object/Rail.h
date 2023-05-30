#pragma once
#include "Header/Object/Object.h"
#include "ViewProjection.h"
#include "math/Lerp.h"
#include "math/Vector3.h"
#include <vector>

class Rail {
	Ray nowRay_;

	Vector3 prePos;
	std::vector<Vector3> drawingPoint_;
	Catmull railPath_;

	float distance_;

	float progress_ = 0.f;
	float defaultSpeed_ = 0.1f;

	float GetDistance(const uint32_t& t) const;

public:
	const Ray& GetRailRay() const { return nowRay_; }

	void Init();
	void AddPoint(const Vector3& pos);
	void AddPoint(const std::vector<Vector3>& pos);
	void CalcDrawPosition(const uint32_t& subDivisions);

	void Update(const float& timeDelta = 1.f);

	void Draw() const;
};