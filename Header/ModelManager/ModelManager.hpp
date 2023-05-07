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
	/// @brief モデルの追加
	/// @param key 文字列キー
	/// @param model モデルデータ
	void AddModel(const std::string& key, Model* model) {
		modelHash hash = std::hash<std::string>{}(key);
		models_[hash] = model;
	}
	/// @brief モデルの追加
	/// @param hash 文字列ハッシュ値
	/// @param model モデルデータ
	void AddModel(const modelHash& hash, Model* model) { models_[hash] = model; }

	/// @brief モデルデータの取得
	/// @param key 文字列キー
	/// @return モデルデータ
	Model* GetModel(const std::string& key) {
		modelHash hash = std::hash<std::string>{}(key);
		return models_[hash];
	}
	/// @brief モデルデータの取得
	/// @param hash 文字列ハッシュ値
	/// @return モデルデータ
	Model* GetModel(const modelHash& hash) { return models_[hash]; }

	/// @brief モデルデータの破棄
	/// @param key 文字列キー
	void removeModel(const std::string& key) {
		modelHash hash = std::hash<std::string>{}(key);
		RemoveModel(hash);
	}

	/// @brief モデル破棄
	/// @param hash 文字列ハッシュ値
	void RemoveModel(const modelHash& hash) {
		delete models_[hash];
		models_.erase(hash);
	}

	/// @brief 全てのモデルデータの破棄
	void RemoveAll() {
		for (auto& element : models_) {
			delete element.second;
		}
		models_.clear();
	}

private:
	ModelManager() {}
	ModelManager(const ModelManager&) = delete;
	// モデルマップ
	std::unordered_map<modelHash, Model*> models_;
};