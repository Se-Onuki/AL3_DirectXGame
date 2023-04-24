#pragma once

#include "Header/Entity/Entity.hpp"
#include "Header/Chunk/Chunk.hpp"
#include "Header/Component/Component.hpp"
#include "Header/World/World.hpp"

class EntityManager {
public:
	EntityManager(World* world);

	template<typename... Ts> 
	Entity CreateEntity() {
		Entity entity{};
		Archetype archetype;
		(archetype.AddTypeInfo<Ts>(), ...);

		auto chunkId = FindMatchChunk(archetype);

		if (chunkId != UINT32_MAX) {
			entity.chunkId_ = chunkId;
			entity.chunkIndex_ = world_->chunkList_[entity.chunkId_].EntityCount();
		} else {
			entity.chunkId_ = world_->chunkList_.size();
			entity.chunkIndex_ = 0;
			Chunk chunk;
			(chunk.AddComponent<Ts>(), ...);
			world_->chunkList_.push_back(chunk);
		}
		world_->chunkList_[entity.chunkId_].entityCount_++;
		return entity;
	}

	uint32_t FindMatchChunk(const Archetype& archetype);
	//std::vector<Chunk&> FindHitChunk(const Archetype& archetype);


	template<typename T> 
	T& GetComponent(Entity entity) {
		return (*world_).chunkList_[entity.chunkId_].Get<T>()[entity.chunkIndex_];
	}

	template<typename T> 
	void SetComponent(Entity entity, const T& component) {
		Chunk chunk = world_->chunkList_[entity.chunkId_];
		T* componentList = chunk.Get<T>();
		if (chunk.entityCount_ == entity.chunkIndex_) {
			//componentList->push_back(component);
			return;
		}
		componentList[entity.chunkIndex_] = component;
	}


private:
	void CheckORCreateChunk() {}
	World* world_;
};

