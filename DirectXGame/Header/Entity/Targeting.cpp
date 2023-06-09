#include "Targeting.h"
#include "Header/Entity/Entity.h"
#include "Header/Object/Object.h"

void Targeting::Update() {
	std::list<Collider*> hitCollider;
	for (auto& collider : colliderList_) {
		const Vector3& closestPoint = mouseSegment_.ClosestPoint(collider->GetPosition());
		if ((closestPoint - collider->GetPosition()).Length() <= collider->GetRadius() + 2.f) {
			hitCollider_ = collider;
			return;
		}
	}
}