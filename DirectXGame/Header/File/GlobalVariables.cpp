#include "GlobalVariables.h"

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
