#pragma once
#include "math/Math.hpp"
#include "math/Matrix4x4.h"
#include "math/Vector3.h"
#include <vector>

struct Sphere {
	Vector3 center;
	float radius;

	bool IsCollision(const Sphere& other) {
		return (center - other.center).Length() <= radius + other.radius;
	}
};

struct LineBase {
	Vector3 origin; // 始点
	Vector3 diff;   // 終点へのベクトル

	[[nodiscard]] Vector3 GetEnd() const { return origin + diff; }
	[[nodiscard]] Vector3 GetProgress(const float& t) const;
	[[nodiscard]] Vector3 Project(const Vector3& point) const;
	[[nodiscard]] Vector3 ClosestPoint(const Vector3& point) const;

protected:
	[[nodiscard]] float ClosestProgress(const Vector3& point) const;
	[[nodiscard]] virtual const float Clamp(const float& t) const = 0;
};

/// @brief 直線
struct Line final : public LineBase {
private:
	[[nodiscard]] const float Clamp(const float& t) const override;
};

/// @brief 半直線
struct Ray final : public LineBase {
private:
	[[nodiscard]] const float Clamp(const float& t) const override;
};

/// @brief 線分
struct Segment final : public LineBase {
private:
	[[nodiscard]] const float Clamp(const float& t) const override;
};

inline bool ChackSphereCollision(
    const Vector3& posA, const float& radiusA, const Vector3& posB, const float& radiusB) {
	return (posA - posB).Length() <= radiusA + radiusB;
}