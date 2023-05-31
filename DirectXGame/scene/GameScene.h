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
#include "Header/Entity/EnemySpawner.h"
#include "Header/Entity/Player.h"
#include "Header/Entity/PlayerBullet.h"
#include "Header/Object/Rail.h"
#include "Header/Object/RailCamera.h"
#include "Header/Object/SkyBox.h"

class CollisionManager;

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

	void AddPlayerBullet(PlayerBullet* bullet) { playerBulletList_.emplace_back(bullet); }
	void AddEnemyBullet(EnemyBullet* bullet) { enemyBulletList_.emplace_back(bullet); }
	void AddEnemy(const Vector3& position);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	ViewProjection viewProjection_;

	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	std::unique_ptr<SkyBox> skyBox_ = nullptr;

	std::unique_ptr<RailCamera> railCamera_ = nullptr;
	std::unique_ptr<Rail> rail_ = nullptr;

	std::unique_ptr<Player> player_;
	std::list<std::unique_ptr<PlayerBullet>> playerBulletList_;

	std::list<std::unique_ptr<Enemy>> enemyList_;
	std::list<std::unique_ptr<EnemyBullet>> enemyBulletList_;

	CollisionManager* collisionManager_ = nullptr;

	std::unique_ptr<EnemySpawner> enemySpawner_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
