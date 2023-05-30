#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <imgui.h>

#include "Header/Entity/CollisionManager.h"
#include "Header/Entity/PlayerBullet.h"
#include "Header/Object/Object.h"

GameScene::GameScene() {}

GameScene::~GameScene() { delete debugCamera_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	rail_.reset(new Rail());
	rail_->AddPoint(std::vector<Vector3>{
	    {0,   0,  0 },
	    {0,   0,  10},
	    {10,  10, 20},
	    {-10, 0,  40},
	    {20, 0,  100},
	});
	rail_->CalcDrawPosition(30);

	railCamera_.reset(new RailCamera());
	railCamera_->Init(Vector3::zero(), Vector3::zero());

	ModelManager::GetInstance()->AddModel("playerModel", Model::Create());
	Model* playerModel = ModelManager::GetInstance()->GetModel("playerModel");

	player_.reset(new Player());
	player_->Init(playerModel, TextureManager::Load("uvChecker.png"));
	player_->SetParent(&railCamera_->GetWorldTransform());
	player_->AddPosition({0, 0, 50.f});

	collisionManager_ = CollisionManager::GetInstance();

	ModelManager::GetInstance()->AddModel("SkyBox", Model::CreateFromOBJ("SkyBox"));
	skyBox_.reset(new SkyBox());
	skyBox_->Init();

	Enemy* enemy = new Enemy();
	enemy->Init(playerModel, TextureManager::Load("white1x1.png"), {1.f, 3.f, 60.f});
	enemy->SetPlayer(player_.get());

	enemyList_.emplace_back(enemy);

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

#pragma region AddCollisionManager
	collisionManager_->clear();

	collisionManager_->push_back(player_.get());
	for (auto& pBullet : player_->GetBullet()) {
		collisionManager_->push_back(pBullet.get());
	}
	for (auto& enemy : enemyList_) {
		collisionManager_->push_back(enemy.get());
	}

	for (auto& enemy : enemyList_) {
		for (auto& eBullet : enemy->GetBullet()) {
			collisionManager_->push_back(eBullet.get());
		}
	}

	collisionManager_->ChackAllCollision();

#pragma endregion

	player_->Update();
	for (auto& enemy : enemyList_) {
		enemy->Update();
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

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}