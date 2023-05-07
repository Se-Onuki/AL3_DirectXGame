#pragma once
#include <string>
#include <unordered_map>

#include "Model.h"

using modelHash = size_t;

class ModelManager {
public:
	static ModelManager& GetInstance() {
		static ModelManager instance;
		return instance;
	}

	~ModelManager() { RemoveAll(); }
	/// @brief ���f���̒ǉ�
	/// @param key ������L�[
	/// @param model ���f���f�[�^
	void AddModel(const std::string& key, Model* model) {
		modelHash hash = std::hash<std::string>{}(key);
		models_[hash] = model;
	}
	/// @brief ���f���̒ǉ�
	/// @param hash ������n�b�V���l
	/// @param model ���f���f�[�^
	void AddModel(const modelHash& hash, Model* model) { models_[hash] = model; }

	/// @brief ���f���f�[�^�̎擾
	/// @param key ������L�[
	/// @return ���f���f�[�^
	Model* GetModel(const std::string& key) {
		modelHash hash = std::hash<std::string>{}(key);
		return models_[hash];
	}
	/// @brief ���f���f�[�^�̎擾
	/// @param hash ������n�b�V���l
	/// @return ���f���f�[�^
	Model* GetModel(const modelHash& hash) { return models_[hash]; }

	/// @brief ���f���f�[�^�̔j��
	/// @param key ������L�[
	void removeModel(const std::string& key) {
		modelHash hash = std::hash<std::string>{}(key);
		RemoveModel(hash);
	}

	/// @brief ���f���j��
	/// @param hash ������n�b�V���l
	void RemoveModel(const modelHash& hash) {
		delete models_[hash];
		models_.erase(hash);
	}

	/// @brief �S�Ẵ��f���f�[�^�̔j��
	void RemoveAll() {
		for (auto& element : models_) {
			delete element.second;
		}
		models_.clear();
	}

private:
	ModelManager() {}
	ModelManager(const ModelManager&) = delete;
	// ���f���}�b�v
	std::unordered_map<modelHash, Model*> models_;
};