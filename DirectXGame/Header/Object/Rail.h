#pragma once
#include "ViewProjection.h"
#include "math/Vector3.h"
#include <vector>

class Rail {

	std::vector<Vector3> controlPoint_;
	std::vector<Vector3> drawingPoint_;

public:
	void Init();
	void AddPoint(const Vector3& pos);
	void AddPoint(const std::vector<Vector3>& pos);
	void GetPosition(const float& t) const;
	void CalcDrawPosition(const uint32_t& subDivisions);

	void Draw() const;
};