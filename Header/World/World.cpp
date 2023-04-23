#include "World.hpp"



World::World() {}

World::~World() {}

EntityManager* World::GetEntityManager() const { return entityManager_; }

void World::Update() {
	//for (auto&& system : systemList_) {
	//	system->onUpdate();
	//}
}