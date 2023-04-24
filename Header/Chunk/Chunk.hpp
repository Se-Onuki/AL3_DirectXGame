#pragma once
#include <typeindex>
#include <typeinfo>
#include <unordered_set>
#include <vector>

#include <assert.h>

#include "Header/Component/Component.hpp"
#include <any>

#include <bit>

#include <stdlib.h>

class Archetype {
public:
	std::unordered_set<std::type_index> typeHash_;
	std::unordered_map<std::type_index, std::size_t> typeSize_;
	std::unordered_map<std::type_index, std::byte[]> typeByte_;

	Archetype() {}

	template<typename... Ts>
	void AddTypeInfo() {
		(typeHash_.insert(std::type_index(typeid(Ts))), ...);
		((typeSize_[std::type_index(typeid(Ts))] = sizeof(Ts)), ...);
	}

	bool operator==(const Archetype& other) const { return typeHash_ == other.typeHash_; }

	bool operator<=(const Archetype& other) const {
		if (typeHash_.size() > other.typeHash_.size()) {
			return false;
		}
		for (const auto& element : typeHash_) {
			if (other.typeHash_.find(element) == other.typeHash_.end()) {
				return false;
			}
		}
		return true;
	}
};

class Chunk {
public:
	template<typename... Ts> Chunk() { (AddComponent<Ts>(), ...); }
	~Chunk() { 
	}

	/*Chunk(const Archetype &archetype) { 
		maxCount_;
	}*/


	template<typename T> void AddComponent();

	void CreateArray(const Archetype& archetype, const uint32_t &maxSize) {
		maxCount_ = maxSize;
		for (auto& element : archetype.typeHash_) {
			componentList_[element] = malloc(archetype_.typeSize_[element] * maxCount_);
		}
	}

	template<typename T> T* Get() {
		return static_cast<T*>(componentList_[std::type_index(typeid(T))]);
	}

	void EntityDelete(uint32_t index) {
		for (auto& element : archetype_.typeHash_) {
			void* compPtr = componentList_[element];
			const uint32_t typeSize = archetype_.typeSize_[element];
			
			
			memcpy((bool*)(compPtr) + index * typeSize, (bool*)compPtr + entityCount_ * typeSize, typeSize);
		}
	}

	void ChankDelete() {
		for (auto& element : componentList_) {
			if (element.second != nullptr) {
				
			}
		}
	}

	const Archetype& GetArchetype() { return archetype_; }

	uint32_t EntityCount() { return entityCount_; }
	uint32_t entityCount_ = 0;
	uint32_t maxCount_ = 1000;

private:
	Archetype archetype_;
	std::unordered_map<std::type_index, void*> componentList_ = {};
};

template<typename T> inline void Chunk::AddComponent() {
	archetype_.AddTypeInfo<T>();
	std::type_index typeData = std::type_index(typeid(T));
	componentList_[typeData] = malloc(archetype_.typeSize_[typeData] * maxCount_);
}