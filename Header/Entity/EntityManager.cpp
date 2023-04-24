#include "Header/Entity/EntityManager.hpp"

#include "Header/World/World.hpp"
#include <limits>

EntityManager::EntityManager(World* world) : world_(world) {}

uint32_t EntityManager::FindMatchChunk(const Archetype& archetype) {
	for (uint32_t i = 0; i < world_->chunkList_.size(); ++i) {
		if (world_->chunkList_[i].GetArchetype() == archetype) {
			return i;
		}
	}
	return UINT32_MAX;
}

//
//std::vector<Chunk&> EntityManager::FindHitChunk(const Archetype& archetype) {
//	std::vector<Chunk&> output = {};
//	for (uint32_t i = 0; i < world_->chunkList_.size(); ++i) {
//		if (archetype <= world_->chunkList_[i].GetArchetype()) {
//			output.push_back(world_->chunkList_[i]);
//		}
//	}
//	return output;
//}
//
