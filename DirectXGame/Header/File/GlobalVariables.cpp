#include "GlobalVariables.h"
#include <imgui.h>

void GlobalVariables::SetValue(
    const std::string& groupName, const std::string& key, const int32_t value) {
	Group& group = datas_[groupName];
	group.items[key] = value;
}

void GlobalVariables::SetValue(
    const std::string& groupName, const std::string& key, const float value) {
	Group& group = datas_[groupName];
	group.items[key] = value;
}

void GlobalVariables::SetValue(
    const std::string& groupName, const std::string& key, const Vector3 value) {
	Group& group = datas_[groupName];
	group.items[key] = value;
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
		for (auto itItem = group.items.begin(); itItem != group.items.end(); itItem++) {
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

		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
	ImGui::End();
}

void GlobalVariables::SaveFile(const std::string& groupName) const {
	// グループ内を検索
	auto itGroup = datas_.find(groupName);
	assert(itGroup != datas_.end());
	
}
