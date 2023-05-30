#include "Rail.h"
#include "PrimitiveDrawer.h"
#include "math/Lerp.h"
#include <imgui.h>

float Rail::GetDistance(const uint32_t& t) const {
	const auto& pointList = railPath_.GetPointList();
	if (pointList.size() == 0)
		return 0.f;
	if (pointList.size() - 1 > t) {
		return (pointList[t + 1] - pointList[t]).Length();
	}
	return 0.f;
}

void Rail::Update(const float& timeDelta) {
	const auto& pointList = railPath_.GetPointList();
	const float preProgress = progress_ + 0.0001f;
	if (preProgress >= pointList.size() - 1) {
		progress_ = 0.f;
		return;
	}
	const Vector3 preFacing =
	    (railPath_.GetStructPosition(preProgress) - railPath_.GetStructPosition(progress_));
	const float distance = preFacing.Length() * 10000.f;

	prePos = nowRay_.origin;
	progress_ += timeDelta * defaultSpeed_ / distance;
	if (progress_ >= pointList.size() - 1)
		progress_ = static_cast<float>(pointList.size() - 1);
	nowRay_.origin = railPath_.GetStructPosition(progress_);
	nowRay_.diff = preFacing.Nomalize();

	float nowSpeed = (nowRay_.origin - prePos).Length();

	ImGui::Begin("rail");
	ImGui::DragFloat(
	    "RailProgress", &progress_, 0.01f, 0.f, static_cast<float>(pointList.size() - 1));
	ImGui::DragFloat("NowSpeed", &nowSpeed, 0.01f);
	ImGui::DragFloat("DefaultSpeed", &defaultSpeed_, 0.01f);
	ImGui::End();
}

void Rail::Init() {
	nowRay_.origin = Vector3::zero();
	nowRay_.diff = Vector3::zero();
	prePos = Vector3::zero();
	progress_ = 0.f;
}

void Rail::AddPoint(const Vector3& pos) { railPath_.AddPoint(pos); }

void Rail::AddPoint(const std::vector<Vector3>& pos) { railPath_.AddPoint(pos); }

void Rail::CalcDrawPosition(const uint32_t& subDivisions) {
	size_t totalLength = railPath_.GetPointList().size() - 1;
	for (size_t i = 0; i <= subDivisions; i++) {
		float t = static_cast<float>(i) / subDivisions * totalLength;
		drawingPoint_.push_back(Catmull::GetStructPosition(railPath_.GetPointList(), t));
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
