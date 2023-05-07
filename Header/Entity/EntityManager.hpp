#pragma once

#include "Header/Chunk/Chunk.hpp"
#include "Header/Component/Component.hpp"
#include "Header/Entity/Entity.hpp"
#include "Header/World/World.hpp"

class EntityManager {
public:
	EntityManager(World* world);

	template<typename... Ts>
	Entity CreateEntity() {
		Entity entity{};
		Archetype archetype;
		archetype.AddClassData<Ts>();

		auto chunkId = world_->FindMatchChunk(archetype);

		if (chunkId == UINT32_MAX) {
			chunkId = world_->CreateChunk(archetype);
		}

		world_->chunkList_[entity.chunkId_].entityCount_++;
		return entity;
	}

	Entity CreateEntity(const uint32_t &chunkId) {
		Chunk* targetChunk = world_->GetChunk(chunkId);
		targetChunk->entityCount_++;
		if (targetChunk->maxCount_ < targetChunk->entityCount_) {
			
		}
		Entity{chunkId, targetChunk->entityCount_};
	}

	// std::vector<Chunk&> FindHitChunk(const Archetype& archetype);

	template<typename T> T& GetComponent(Entity entity) {
		return world_->chunkList_[entity.chunkId_].Get<T>()[entity.chunkIndex_];
	}

	template<typename T> void SetComponent(Entity entity, T& component) {
		Chunk* chunk = world_->GetChunk(entity.chunkId_);
		T* componentList = chunk->Get<T>();
		if (chunk->maxCount_ == entity.chunkIndex_) {
		}

		new (&componentList[entity.chunkIndex_]) T(component);
	}

private:
	void CheckORCreateChunk() {}
	World* world_;
};
