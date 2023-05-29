#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "DebugCamera.h"
#include "Header/Entity/Enemy.h"
#include "Header/Entity/Player.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	ViewProjection viewProjection_;

	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	std::unique_ptr<Player> player_;
	std::list<std::unique_ptr<Enemy>> enemyList_;

	/// @brief 衝突判定と応答
	void ChackAllCollision();

	void CheckCollisionPair(Collider* A, Collider* B) const;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
