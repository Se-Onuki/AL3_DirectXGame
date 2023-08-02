#include "GlobalVariables.h"
// #include <WinUser.h>
#include <fstream>
#include <imgui.h>
#include <json.hpp>
#include <windows.h>

#include "WinApp.h"

void GlobalVariables::SetValue(
    const std::string& groupName, const std::string& key, const int32_t value) {
	Group& group = datas_[groupName];
	group[key] = value;
}

void GlobalVariables::SetValue(
    const std::string& groupName, const std::string& key, const float value) {
	Group& group = datas_[groupName];
	group[key] = value;
}

void GlobalVariables::SetValue(
    const std::string& groupName, const std::string& key, const Vector3 value) {
	Group& group = datas_[groupName];
	group[key] = value;
}

void GlobalVariables::Update() {
	if (!ImGui::Begin("GlobalVariables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar()) {
		return;
	}

	for (auto itGroup = datas_.begin(); itGroup != datas_.end(); itGroup++) {
		const std::string& groupName = itGroup->first;

		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str())) {
			continue;
		}
		for (auto itItem = group.begin(); itItem != group.end(); itItem++) {
			const std::string& itemName = itItem->first;

			Item& item = itItem->second;

			if (std::holds_alternative<int32_t>(item)) {
				int32_t* ptr = std::get_if<int32_t>(&item);
				ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
			} else if (std::holds_alternative<float>(item)) {
				float* ptr = std::get_if<float>(&item);
				ImGui::SliderFloat(itemName.c_str(), ptr, 0.f, 100.f);
			} else if (std::holds_alternative<Vector3>(item)) {
				Vector3* ptr = std::get_if<Vector3>(&item);
				ImGui::SliderFloat3(itemName.c_str(), &ptr->x, -10.f, -10.f);
			}
		}

		ImGui::Text("\n");
		if (ImGui::Button("Save")) {

			SaveFile(groupName);
			std::string message = std::format("{}.json saved", groupName);
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		}

		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
	ImGui::End();
}

void GlobalVariables::SaveFile(const std::string& groupName) const {
	// グループ内を検索
	auto itGroup = datas_.find(groupName);
	assert(itGroup != datas_.end());

	nlohmann::json root;                        // ""空
	root = nlohmann::json::object();            // "{}" キー無しのJson構造体を作成
	root[groupName] = nlohmann::json::object(); // "{"groupName":{}}"オブジェクト生成

	for (auto itItem = itGroup->second.begin(); itItem != itGroup->second.end();
	     itItem++) {
		const std::string& itemName = itItem->first; // キー
		const Item& item = itItem->second;           // Value

		if (std::holds_alternative<int32_t>(item)) {
			root[groupName][itemName] = std::get<int32_t>(item);
		} else if (std::holds_alternative<float>(item)) {
			root[groupName][itemName] = std::get<float>(item);
		} else if (std::holds_alternative<Vector3>(item)) {
			const Vector3& value = std::get<Vector3>(item);
			root[groupName][itemName] = nlohmann::json::array({value.x, value.y, value.z});
		}
	}

	// ディレクトリが無ければ作成する
	std::filesystem::path dir{kDirectoryPath};
	if (!std::filesystem::exists(kDirectoryPath)) {
		std::filesystem::create_directory(kDirectoryPath);
	}

	// 書き込み対象のJsonファイルを作成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	std::ofstream ofs;
	ofs.open(filePath);

	// ファイルオープン失敗の時
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	ofs.close();
}
