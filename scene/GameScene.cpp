#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <imgui.h>

#include "Math.hpp"

#include "Header/Component/Component.hpp"
#include "Header/Entity/EntityManager.hpp"
#include "Header/World/World.hpp"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("Player/Player.png");

	EntityManager* eManager = world.GetEntityManager();
	Entity entity = eManager->CreateEntity<PositionComp, VelocityComp>();
	PositionComp pos;
	pos.poistion_ = {100, 1003};
	VelocityComp velo;
	velo.velocity_ = {1,-10};
	eManager->SetComponent(entity, pos);
	eManager->SetComponent(entity, velo);

	auto& test = eManager->GetComponent<PositionComp>(entity);
}

void GameScene::Update() {
	world.ForEach<PositionComp, VelocityComp>([](PositionComp& pos, VelocityComp& velo) { pos.poistion_ += velo.velocity_; });
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
