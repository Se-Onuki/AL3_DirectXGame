#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>
#include <imgui.h>

#include "Header/Entity/PlayerBullet.h"
#include "Header/Object/Object.h"

GameScene::GameScene() {}

GameScene::~GameScene() { delete debugCamera_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	ModelManager::GetInstance()->AddModel("playerModel", Model::Create());
	Model* playerModel = ModelManager::GetInstance()->GetModel("playerModel");

	player_.reset(new Player());
	player_->Init(playerModel, TextureManager::Load("uvChecker.png"));

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
	ChackAllCollision();
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
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
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

void GameScene::ChackAllCollision() {

	const Vector3& playerPosition = player_->GetPosition();
	const float& playerRadius = player_->collisionRadius;
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullet();

#pragma region 自キャラと敵弾との当たり判定

	for (auto& enemy : enemyList_) {
		for (auto& enemyBullet : enemy->GetBullet()) {
			if (ChackSphereCollision(
			        playerPosition, playerRadius, enemyBullet->GetPosition(),
			        enemyBullet->collisionRadius)) {
				player_->OnCollision();
				enemyBullet->OnCollision();
			}
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラとの当たり判定

	for (auto& enemy : enemyList_) {
		for (auto& pBullet : playerBullets) {
			if (ChackSphereCollision(
			        pBullet->GetPosition(), pBullet->collisionRadius, enemy->GetPosition(),
			        enemy->collisionRadius)) {
				enemy->OnCollision();
				pBullet->OnCollision();
			}
		}
	}

#pragma endregion

#pragma region 自弾と敵弾との当たり判定

	for (auto& pBullet : playerBullets) {
		for (auto& enemy : enemyList_) {
			for (auto& eBullet : enemy->GetBullet()) {
				if (ChackSphereCollision(
				        pBullet->GetPosition(), pBullet->collisionRadius, eBullet->GetPosition(),
				        eBullet->collisionRadius)) {
					pBullet->OnCollision();
					eBullet->OnCollision();
				}
			}
		}
	}

#pragma endregion
}
