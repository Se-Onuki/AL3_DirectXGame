#pragma once
#include <typeindex>
#include <typeinfo>
#include <unordered_set>
#include <vector>

#include <assert.h>

#include "Header/Component/Component.hpp"
#include <any>


class Archetype {
public:
	std::unordered_set<std::type_index> typeHash_;

	Archetype() {}

	template<typename... Ts>
	void AddTypeInfo() {
		(typeHash_.insert(std::type_index(typeid(Ts))), ...);
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

	template<typename T> void AddComponent() {
		componentList_[std::type_index(typeid(T))] = std::vector<T>();
		archetype_.AddTypeInfo<T>();
	}

	template<typename T> std::vector<T>& Get() {
		return std::any_cast<std::vector<T>&>(componentList_[std::type_index(typeid(T))]);
	}

	~Chunk() {}

	const Archetype& GetArchetype() { return archetype_; }

private:
	Archetype archetype_;
	std::unordered_map<std::type_index, std::any> componentList_;
};