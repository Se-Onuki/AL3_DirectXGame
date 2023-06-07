#pragma once
#include "Header/Object/Object.h"
#include <list>

class Collider;

class Targeting {
	std::list<Collider*> colliderList_;
	Segment mouseSegment_ = {};

	Collider* hitCollider_ = nullptr;

	Targeting() {}
	Targeting(const Targeting&) = delete;
	Targeting operator=(const Targeting&) = delete;

public:
	static Targeting* GetInstance() {
		static Targeting Targeting;
		return &Targeting;
	}

	void clear() {
		colliderList_.clear();
		hitCollider_ = nullptr;
	}
	void push_back(Collider* collider) { colliderList_.push_back(collider); }
	void SetSegment(const Segment& segment) { mouseSegment_ = segment; }

	Collider * const GetHitCollider() const { return hitCollider_; }

	void Update();
};