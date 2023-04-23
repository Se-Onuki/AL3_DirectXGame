#pragma once

#include "Header/Entity/Entity.hpp"

class World;
class Archetype;

class EntityManager {
public:
	EntityManager(World& world) : world_(world) {}

	template<typename... Ts>
	Entity CreateEntity() {
		Entity entity(*this);
		Archetype archetype;
		(archetype.AddTypeInfo<Ts>(), ...);

		auto chunkId = FindChunk(archetype);
		if (chunkId != std::numeric_limits<uint32_t>::max()) {
			entity.chunkId_ = chunkId;
			entity.chunkIndex_ = world_.chunkList_[entity.chunkId_].Size();
		} else {
			entity.chunkId_ = world_.chunkList_.size();
			entity.chunkIndex_ = 0;
			Chunk chunk(archetype);
			(chunk.AddComponent<Ts>(), ...);
			world_.chunkList_.push_back(chunk);
		}
		return entity;
	}

	uint32_t FindChunk(const Archetype& archetype);

	template<typename T> T& GetComponent(Entity entity) {
		return world_.chunkList_[entity.chunkId_].Get<T>()[entity.chunkIndex_];
	}

private:
	World& world_;
};