#include "EntityManager.hpp"

#include "Header/World/World.hpp"

uint32_t EntityManager::FindChunk(const Archetype& archetype) {
	for (uint32_t i = 0; i < world_.chunkList_.size(); ++i) {
		if (world_.chunkList_[i].GetArchetype() == archetype) {
			return i;
		}
	}
	return std::numeric_limits<uint32_t>::max();
}