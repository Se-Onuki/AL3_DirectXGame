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

class Entity;

class ClassData {
public:
	std::type_index type_;
	size_t size_;
	void (*constructor_)(void*);
	void (*destructor_)(void*);

	template<typename T> static ClassData Create() {
		return {
		    std::type_index(typeid(T)), sizeof(T), [](void* ptr) { new (ptr) T(); },
		    [](void* ptr) { (static_cast<T*>(ptr))->~T(); }};
	}
};

class MyArray {
public:
	MyArray(const ClassData& classData, uint32_t size = 1)
	    : maxSize_(size), data_(nullptr), classData_(classData), nowSize_(0) {
		data_ = malloc(size * classData_.size_);
		for (uint32_t i = 0; i < size; i++) {
			void* ptr = static_cast<char*>(data_) + i * classData_.size_;
			classData_.constructor_(ptr);
		}
	}

	~MyArray() {
		if (data_) {
			for (uint32_t i = 0; i < maxSize_; i++) {
				void* ptr = static_cast<char*>(data_) + i * classData_.size_;
				classData_.destructor_(ptr);
			}
			free(data_);
		}
	}

	bool Resize(uint32_t newSize) {
		if (newSize <= maxSize_) {
			return true;
		}
		void* newData = realloc(data_, newSize * classData_.size_);
		if (!newData) {
			return false;
		}
		data_ = newData;
		for (uint32_t i = maxSize_; i < newSize; i++) {
			void* ptr = static_cast<char*>(data_) + i * classData_.size_;
			classData_.constructor_(ptr);
		}
		maxSize_ = newSize;
		return true;
	}

	template<typename T>
	void push_back(const T& value) {
		if (nowSize_ == maxSize_) {
			Resize(maxSize_ * 2);
		}
		void* ptr = static_cast<char*>(data_) + nowSize_ * classData_.size_;
		new (ptr) T(value);
		nowSize_++;
	}

	/// @brief óvëfÇÃì¸ÇÍë÷Ç¶
	/// @param index1 óvëfA
	/// @param index2 óvëfB
	/// @return ì¸ÇÍë÷Ç¶ÇÃê¨î€
	bool swap(uint32_t index1, uint32_t index2) {
		if (index1 >= nowSize_ || index2 >= nowSize_) {
			return false;
		}
		void* ptr1 = static_cast<char*>(data_) + index1 * classData_.size_;
		void* ptr2 = static_cast<char*>(data_) + index2 * classData_.size_;
		void* swapBuff = malloc(classData_.size_);
		memcpy(swapBuff, ptr1, classData_.size_);
		memcpy(ptr1, ptr2, classData_.size_);
		memcpy(ptr2, swapBuff, classData_.size_);
		free(swapBuff);
		return true;
	}

	/// @brief óvëfÇÃçÌèú
	/// @param index çÌèúÇ∑ÇÈìYÇ¶éö
	/// @return çÌèúÇÃê¨î€
	bool erase(uint32_t index) {
		if (empty() || index >= nowSize_) { // ï€ë∂ó Ç™0 or ìYÇ¶éöÇ™ÉIÅ[ÉoÅ[ÇµÇƒÇ¢ÇÈéûÇÕñ≥å¯
			return false;
		}
		void* ptr = static_cast<char*>(data_) + index * classData_.size_;
		classData_.destructor_(ptr);
		memmove(
		    ptr, static_cast<char*>(ptr) + classData_.size_,
		    (nowSize_ - index - 1) * classData_.size_);
		nowSize_--;
		return true;
	}
	
	/// @brief óvëfÇÃì¡éÍçÌèú
	/// @param index çÌèúÇ∑ÇÈìYÇ¶éö
	/// @return çÌèúÇÃê¨î€
	bool swapErase(uint32_t index) {
		if (empty() || index >= nowSize_) {
			return false;
		}

		void* ptr = static_cast<char*>(data_) + index * classData_.size_;
		classData_.destructor_(ptr);

		void* lastPtr = static_cast<char*>(data_) + (nowSize_ - 1) * classData_.size_;
		memcpy(ptr, lastPtr, classData_.size_);

		nowSize_--;
		return true;
	}

	void passElement(MyArray& otherArray, uint32_t index) {
		if (empty() || index >= nowSize_) {
			return;
		}

		void* ptr = static_cast<char*>(data_) + index * classData_.size_;
		otherArray.push_back(*static_cast<int*>(ptr));

		swapErase(index);
	}
	

	void* operator[](const uint32_t& index) {
		if (empty() || index >= nowSize_) {
			return nullptr;
		}
		return static_cast<char*>(data_) + classData_.size_ * index;
	}

	
	void* begin() const { return data_; }
	void* end() const {
		if (empty())
			return data_;
		return static_cast<char*>(data_) + classData_.size_ * (nowSize_ - 1);
	}

	uint32_t size() const { return nowSize_; }

	bool empty() const { return nowSize_ == 0; }

	template<typename T> T* Get() { return static_cast<T*>(data_); }

private:
	uint32_t maxSize_;
	void* data_;
	ClassData classData_;
	uint32_t nowSize_;
};

class Archetype {
public:
	std::unordered_map<std::type_index, ClassData> classData_;

	Archetype() {}

	template<typename... Ts> void AddClassData() {
		(classData_[typeid(Ts...)] = ClassData::SetClass<Ts...>());
	}

	bool operator==(const Archetype& other) const {
		if (classData_.size() != other.classData_.size())
			return false;
		for (const auto& [key, value] : classData_) {
			if (other.classData_.count(key) == 0)
				return false;
		}
		return true;
	}

	bool operator<=(const Archetype& other) const {
		if (classData_.size() > other.classData_.size())
			return false;
		for (const auto& [key, value] : classData_) {
			if (other.classData_.count(key) == 0)
				return false;
		}
		return true;
	}
};

class Chunk {
public:
	Chunk(const Archetype& archetype, const uint32_t& maxSize = 1) { CreateArray(archetype); }
	Chunk(const Chunk& otherChunk, const uint32_t& maxSize = 1)
	    : archetype_(otherChunk.archetype_), maxCount_(maxSize) {
		std::unordered_map<std::type_index, std::pair<void*, void (*)(void*, uint32_t)>>
		    componentList_ = {};
	}

	~Chunk() {
		for (const auto& [key, data] : componentList_) {
			data.second(data.first, maxCount_);
		}
	}

	void CopyChunk(Chunk& other) {
		other.entityCount_ = entityCount_;
		other.archetype_ = archetype_;
		// for ()
	}

	void CreateArray(const Archetype& archetype) {
		for (const auto& [key, classData] : archetype.classData_) {
			componentList_[key] = {malloc(classData.size_ * maxCount_), classData.deleter_};
		}
	}

	template<typename T> T* Get() {
		return static_cast<T*>(componentList_[std::type_index(typeid(T))]);
	}

	Entity CreateEntity();

	void ChankDelete() {}

	const Archetype& GetArchetype() { return archetype_; }

	const uint32_t& size() { return entityCount_; }
	uint32_t entityCount_ = 0;
	const uint32_t maxCount_ = 1;

private:
	Archetype archetype_;
	std::unordered_map<std::type_index, std::pair<void*, void (*)(void*, uint32_t)>>
	    componentList_ = {};
};

// class CustomArray {
// public:
//	CustomArray(const ClassData &classData) {
//
//	}
//
//
// private:
//	void* array_;
//	uint32_t classSizeof_;
// };
