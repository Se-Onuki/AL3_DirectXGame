#pragma once
#include <stdint.h>
#include <Vector3.h>
#include <string>
#include <unordered_map>
#include <variant>

class GlobalVariables {
	GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	GlobalVariables operator=(const GlobalVariables&) = delete;
	~GlobalVariables() = default;

public:
	/*struct Item {
		std::variant<int32_t, float, Vector3> value;
	};*/

	using Item = std::variant<int32_t, float, Vector3> ;

	struct Group {
		std::unordered_map<std::string, Item> items;
	};

	static GlobalVariables* const GetInstance() {
		static GlobalVariables instance{};
		return &instance;
	}

	void CreateGroups(const std::string& groupName) { datas_[groupName]; }

	void SetValue(const std::string& groupName, const std::string& key, const int32_t value);
	void SetValue(const std::string& groupName, const std::string& key, const float value);
	void SetValue(const std::string& groupName, const std::string& key, const Vector3 value);

private:
	std::unordered_map<std::string, Group> datas_;
};