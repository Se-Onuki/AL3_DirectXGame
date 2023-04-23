#pragma once
#include <cstdint>

class Entity {
public:
	uint32_t chunkId_;
	uint32_t chunkIndex_;

	//template<typename T> T& GetComp();
};
