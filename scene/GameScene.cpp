#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <imgui.h>

#include <random>

#include "Math.hpp"

#include "Header/Component/Component.hpp"
#include "Header/Entity/EntityManager.hpp"
#include "Header/World/World.hpp"

GameScene::GameScene() {}

GameScene::~GameScene() { delete sprite_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("uvChecker.png");
	sprite_ = Sprite::Create(textureHandle_, {0,0});

	EntityManager* eManager = world.GetEntityManager();

	Entity entity = eManager->CreateEntity<VelocityComp, SpriteComp>();
	VelocityComp velo;
	velo.velocity_ = {};
	//SpriteComp sprite(Sprite::Create(textureHandle_, {0, 0}));
	eManager->SetComponent(entity, velo);
	//eManager->SetComponent(entity, sprite);
}

void GameScene::Update() {
/*	world.ForEach<SpriteComp, VelocityComp>([](SpriteComp& sprite, VelocityComp& velo) {
		sprite.sprite_->SetPosition(sprite.sprite_->GetPosition() + velo.velocity_);
	});*/
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
	//world.ForEach<SpriteComp>([](SpriteComp& sprite) { sprite.sprite_->Draw(); });
	sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
