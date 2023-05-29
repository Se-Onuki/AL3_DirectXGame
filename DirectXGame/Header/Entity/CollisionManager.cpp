#include "CollisionManager.h"
#include "Header/Entity/Enemy.h"

void CollisionManager::ChackAllCollision() {

	Collider *colliderA, *colliderB;
	std::list<Collider*>::iterator itrA = colliderList_.begin();
	for (; itrA != colliderList_.end(); ++itrA) {
		colliderA = *itrA;
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliderList_.end(); ++itrB) {
			colliderB = *itrB;
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* A, Collider* B) {
	auto test = (A->GetCollisionAttribute() & B->GetCollisionMask());
	test = test;
	if ((A->GetCollisionAttribute() & B->GetCollisionMask()) == 0u ||
	    (B->GetCollisionAttribute() & A->GetCollisionMask()) == 0u)
		return;
	if ((A->GetPosition() - B->GetPosition()).Length() <= A->GetRadius() + B->GetRadius()) {
		A->OnCollision();
		B->OnCollision();
	}
}
