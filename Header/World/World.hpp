#pragma once
#include <list>
#include "Header/Entity/Entity.hpp"
#include "Header/Chunk/Chunk.hpp"

class SystemBase;
class EntityManager;

class World {
public:
	World() : entityManager_(*this) {}

	EntityManager& GetEntityManager() { return entityManager_; }

	EntityManager entityManager_;
	std::vector<Chunk> chunkList_;
	
};