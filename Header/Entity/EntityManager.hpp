//#pragma once
//
//#include "Header/Entity/Entity.hpp"
//
//class EntityManager {
//public:
//	static EntityManager& Instance() {
//		static EntityManager instance;
//		return instance;
//	}
//
//	void AddEntity() {
//		for (uint32_t i = lastEntityIndex; i < entities.size(); ++i) {
//			if (!entities[i]) {
//				entities[i] = true;
//				lastEntityIndex = i;
//				return;
//			}
//		}
//		entities.push_back(true);
//		lastEntityIndex = entities.size() - 1;
//	}
//
//	void removeEntity(uint32_t index) {
//		if (index < entities.size()) {
//			entities[index] = false;
//			if (index < lastEntityIndex) {
//				lastEntityIndex = index;
//			}
//		}
//	}
//
//	//void printEntities() const {
//	//	std::cout << "entities: [";
//	//	for (const auto& entity : entities) {
//	//		std::cout << entity << ", ";
//	//	}
//	//	std::cout << "]" << std::endl;
//	//	std::cout << lastEntityIndex << std::endl;
//	//}
//
//private:
//	EntityManager() : lastEntityIndex(0) {}
//	EntityManager(const EntityManager&) = delete;
//	EntityManager& operator=(const EntityManager&) = delete;
//
//	std::vector<bool> entities;
//	uint32_t lastEntityIndex;
//};