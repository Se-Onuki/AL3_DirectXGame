#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <imgui.h>

#include "Header/Entity/CollisionManager.h"
#include "Header/Entity/PlayerBullet.h"
#include "Header/Entity/Targeting.h"
#include "Header/Object/Object.h"
#include "Header/Render/Render.hpp"

GameScene::GameScene() {}

GameScene::~GameScene() { delete debugCamera_; }

void GameScene::AddEnemy(const Vector3& position) {
	Model* box = ModelManager::GetInstance()->GetModel("playerModel");
	Enemy* enemy = new Enemy();
	enemy->Init(box, TextureManager::Load("white1x1.png"), position);
	enemy->SetPlayer(player_.get());
	enemy->SetGameScene(this);

	enemyList_.emplace_back(enemy);
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	rail_.reset(new Rail());
	rail_->Init();
	rail_->AddPoint(std::vector<Vector3>{
	    {0,   0,   0   },
	    {0,   0,   20  },
	    {5,   0,   50  },
	    {10,  0,   100 },
	    {-10, 50,  200 },
	    {-50, 20,  200 },
	    {0,   -30, -300},
	    {100, -10, -300},
	    {0,   0,   0   },
	});
	rail_->CalcDrawPosition(30);

	railCamera_.reset(new RailCamera());
	railCamera_->Init(Vector3::zero(), Vector3::zero());
	railCamera_->SetRail(rail_.get());

	TextureManager::Load("target.png");

	ModelManager::GetInstance()->AddModel("playerModel", Model::Create());
	Model* playerModel = ModelManager::GetInstance()->GetModel("playerModel");

	player_.reset(new Player());
	player_->Init(playerModel, TextureManager::Load("uvChecker.png"));
	player_->SetParent(&railCamera_->GetWorldTransform());
	player_->AddPosition({0, 0, 50.f});
	player_->SetGameScene(this);

	targeting_ = Targeting::GetInstance();

	collisionManager_ = CollisionManager::GetInstance();

	ModelManager::GetInstance()->AddModel("SkyBox", Model::CreateFromOBJ("SkyBox"));
	skyBox_.reset(new SkyBox());
	skyBox_->Init();

	enemySpawner_.reset(new EnemySpawner());
	enemySpawner_->Init();
	enemySpawner_->SetGameScene(this);
	enemySpawner_->LoadCSV("./Resources/enemyPop.csv");

	viewProjection_.Initialize();
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {

	railCamera_->Update();
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
	viewProjection_.TransferMatrix();

	playerBulletList_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (bullet->IsDead()) {
			bullet.reset();
			return true;
		}
		return false;
	});

	enemyList_.remove_if([](std::unique_ptr<Enemy>& enemy) {
		if (enemy->IsDead()) {
			enemy.reset();
			return true;
		}
		return false;
	});

	enemyBulletList_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->IsDead()) {
			bullet.reset();
			return true;
		}
		return false;
	});

#pragma region AddCollisionManager
	collisionManager_->clear();

	collisionManager_->push_back(player_.get());
	for (auto& pBullet : playerBulletList_) {
		collisionManager_->push_back(pBullet.get());
	}
	for (auto& enemy : enemyList_) {
		collisionManager_->push_back(enemy.get());
	}

	for (auto& eBullet : enemyBulletList_) {
		collisionManager_->push_back(eBullet.get());
	}

	collisionManager_->ChackAllCollision();

#pragma endregion

#pragma region Target

	targeting_->clear();
	for (auto& enemy : enemyList_) {
		targeting_->push_back(enemy.get());
	}
	targeting_->Update();

#pragma endregion

	enemySpawner_->Update();

	player_->Update(viewProjection_);
	for (auto& playerBullet : playerBulletList_) {
		playerBullet->Update();
	}
	for (auto& enemy : enemyList_) {
		enemy->Update();
	}
	for (auto& enemyBullet : enemyBulletList_) {
		enemyBullet->Update();
	}

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

#endif // _DEBUG
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView *= debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		// viewProjection_.UpdateMatrix();
	}

	skyBox_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection_);
	for (auto& enemy : enemyList_) {
		enemy->Draw(viewProjection_);
	}
	for (auto& playerBullet : playerBulletList_) {
		playerBullet->Draw(viewProjection_);
	}
	for (auto& enemyBullet : enemyBulletList_) {
		enemyBullet->Draw(viewProjection_);
	}
	skyBox_->Draw(viewProjection_);

	rail_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}