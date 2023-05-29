#include "Object.h"
#include "math/Lerp.h"

Vector3 LineBase::GetProgress(const float& t) const { return Lerp(origin, GetEnd(), t); }

Vector3 LineBase::Project(const Vector3& point) const {
	return (point - origin) - ClosestProgress(point) * diff;
}

Vector3 LineBase::ClosestPoint(const Vector3& point) const {
	return ClosestProgress(point) * diff + origin;
}

float LineBase::ClosestProgress(const Vector3& point) const {
	return Clamp(((point - origin) * diff) / std::powf(diff.Length(), 2));
}

const float Line::Clamp(const float& t) const { return t; }

const float Ray::Clamp(const float& t) const { return (t > 0.f) ? t : 0.f; }

const float Segment::Clamp(const float& t) const { return (t > 0.f) ? t : (t < 1.f) ? t : 1.f; }
