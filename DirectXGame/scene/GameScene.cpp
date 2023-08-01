#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>
#include <imgui.h>

#include "Header/Entity/Enemy.h"
#include "Header/Entity/FollowCamera.h"
#include "Header/ModelManager/ModelManager.h"
#include "Header/Object/Ground.h"
#include <Math.hpp>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	Model* const playerBody =
	    ModelManager::GetInstance()->AddModel("playerBody", Model::CreateFromOBJ("body"));
	Model* const playerHead =
	    ModelManager::GetInstance()->AddModel("playerHead", Model::CreateFromOBJ("head"));
	Model* const playerLeft =
	    ModelManager::GetInstance()->AddModel("playerLeft", Model::CreateFromOBJ("left"));
	Model* const playerRight =
	    ModelManager::GetInstance()->AddModel("playerRight", Model::CreateFromOBJ("right"));
	ModelManager::GetInstance()->AddModel("Ground", Model::CreateFromOBJ("Ground"));

	Model* const enemyBody =
	    ModelManager::GetInstance()->AddModel("enemyBody", Model::CreateFromOBJ("needle_Body"));

	std::unordered_map<std::string, Model*> playerMap_{
	    {"body",  playerBody },
	    {"head",  playerHead },
	    {"right", playerRight},
	    {"left",  playerLeft },
	};

	player_.reset(new Player());
	player_->Init(playerMap_);

	enemy_.reset(new Enemy);
	enemy_->Init(std::unordered_map<std::string, Model*>{
	    {"body", enemyBody}
    });

	ground_.reset(new Ground);
	ground_->Init();

	followCamera_.reset(new FollowCamera);
	followCamera_->Init();
	followCamera_->SetTarget(&player_->GetWorldTransform());
	player_->SetViewProjection(followCamera_->GetViewProjection());

	viewProjection_.Initialize();
	viewProjection_.rotation_.x += 10.f * Angle::Dig2Rad;
	viewProjection_.translation_.y += 10.f;
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
	debugCamera_.reset(new DebugCamera(1280, 720));
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	enemy_->Update();
	player_->Update();
	followCamera_->Update();

	viewProjection_.matView = followCamera_->GetViewMatrix();
	viewProjection_.matProjection = followCamera_->GetProjectionMatrix();

	viewProjection_.TransferMatrix();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}

#endif // _DEBUG
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
	enemy_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);

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
