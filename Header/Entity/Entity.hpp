#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cstdint>

class Entity {
	// - ワールド変換データ
	WorldTransform worldTransform_;
	// - モデル
	Model *model_;
	// - テクスチャハンドル
	uint32_t textureHandle_ = 0u;

public:
	/// @brief 初期化
	void Init();
	/// @brief 更新
	void Update();
	/// @brief 描画
	void Draw();
};
