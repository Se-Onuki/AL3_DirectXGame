#pragma once
#include <Vector3.h>
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
	/*struct Item {
	    std::variant<int32_t, float, Vector3> value;
	};*/

	using Item = std::variant<int32_t, float, Vector3>;

	using Group = std::unordered_map<std::string, Item>;

	static GlobalVariables* const GetInstance() {
		static GlobalVariables instance{};
		return &instance;
	}

	void CreateGroups(const std::string& groupName) { datas_[groupName]; }

	void AddValue(const std::string& groupName, const std::string& key, const int32_t value);

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