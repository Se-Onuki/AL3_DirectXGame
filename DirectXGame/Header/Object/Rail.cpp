#include "Rail.h"
#include "PrimitiveDrawer.h"
#include "math/Lerp.h"
void Rail::Init() {}

void Rail::AddPoint(const Vector3& pos) { controlPoint_.push_back(pos); }

void Rail::AddPoint(const std::vector<Vector3>& pos) {
	controlPoint_.insert(controlPoint_.end(), pos.begin(), pos.end());
}

void Rail::GetPosition(const float& t) const { t; }

void Rail::CalcDrawPosition(const uint32_t& subDivisions) {
	size_t totalLength = controlPoint_.size() - 1;
	for (size_t i = 0; i < subDivisions; i++) {
		float t = static_cast<float>(i) / subDivisions * totalLength;
		drawingPoint_.push_back(Catmull::GetStructPosition(controlPoint_, t));
	}
}

void Rail::Draw() const {
	if (drawingPoint_.size() == 0)
		return;
	for (size_t i = 0; i < drawingPoint_.size() - 1; i++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
		    drawingPoint_[i], drawingPoint_[i + 1], Vector4{1, 0, 0, 1});
	}
}
