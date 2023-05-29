#pragma once
#include <list>
class Collider;

class CollisionManager {
	std::list<Collider*> colliderList_;
	CollisionManager() {}
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager operator=(const CollisionManager&) = delete;

public:
	static CollisionManager* GetInstance() {
		static CollisionManager collisionManager;
		return &collisionManager;
	}
	void clear() { colliderList_.clear(); }
	void push_back(Collider* collider) { colliderList_.push_back(collider); }

	void ChackAllCollision();
	void CheckCollisionPair(Collider* A, Collider* B);
};