#pragma once
#include <Vector3.h>
#include <assert.h>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <variant>

class GlobalVariables {
	GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	GlobalVariables operator=(const GlobalVariables&) = delete;
	~GlobalVariables() = default;

	const std::string kDirectoryPath = "Resources/GlobalVariables/";

public:
	using Item = std::variant<int32_t, float, Vector3>;

	using Group = std::unordered_map<std::string, Item>;

	static GlobalVariables* const GetInstance() {
		static GlobalVariables instance{};
		return &instance;
	}

	void CreateGroups(const std::string& groupName) { datas_[groupName]; }

	template<typename T> T Get(const std::string& groupName, const std::string& key) const {
		// グループ内を検索
		auto itGroup = datas_.find(groupName);
		assert(itGroup != datas_.end());

		// キーがあるか
		auto itItem = itGroup->second.find(key);
		assert(itItem != itGroup->second.end());

		// 型が正しいか
		if (std::holds_alternative<T>(itItem->second)) {
			return std::get<T>(itItem->second);
		}
		assert(0 && "存在しない型を取得しようとしました。");
		return T{};
	}
	/*template<typename T>
	void AddValue(const std::string& groupName, const std::string& key, const T value);*/

	void AddValue(const std::string& groupName, const std::string& key, const int32_t value);
	void AddValue(const std::string& groupName, const std::string& key, const float value);
	void AddValue(const std::string& groupName, const std::string& key, const Vector3 value);

	void SetValue(const std::string& groupName, const std::string& key, const int32_t value);
	void SetValue(const std::string& groupName, const std::string& key, const float value);
	void SetValue(const std::string& groupName, const std::string& key, const Vector3 value);

	void Update();

	void SaveFile(const std::string& groupName) const;

	void LoadFile();
	void LoadFile(const std::string& groupName);

private:
	std::unordered_map<std::string, Group> datas_;
};

//class Serializer {
//	std::unordered_map<std::string, std::pair<GlobalVariables::Item, void*>> itemMap_;
//
//public:
//	Serializer() = default;
//	~Serializer() = default;
//
//	/// @brief 入出力データ設定
//	/// @tparam T 保存する型
//	/// @param key
//	/// @param item
//	template<typename T> void AddItem(const std::string& key, T* const item) {
//		itemMap_[key] = {*item, item};
//	}
//
//	void SetItems(const std::string& groupName) const;
//
//private:
//};
//
//template<typename T>
//inline void
//    GlobalVariables::AddValue(const std::string& groupName, const std::string& key, const T value) {
//	auto itItem = datas_[groupName].find(key); // キーがあるか
//	if (itItem == datas_[groupName].end()) {
//		SetValue(groupName, key, value);
//		return;
//	}
//	return;
//}
