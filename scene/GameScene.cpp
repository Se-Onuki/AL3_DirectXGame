#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <imgui.h>

#include <cassert>
#include <random>

#include "Math.hpp"

#include "Header/Component/Component.hpp"
#include "Header/Entity/EntityManager.hpp"
#include "Header/World/World.hpp"

#include "Header/ModelManager/ModelManager.hpp"

GameScene::GameScene() { world = World::GetInstance(); }

GameScene::~GameScene() {}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("uvChecker.png");

	viewProjection_.Initialize();
	worldTransform_.Initialize();

	EntityManager* eManager = world->GetEntityManager();

	Entity entity = eManager->CreateEntity<TransformComp, ModelComp, TextureComp>();
	TransformComp transform;
	transform.wTransform_.Initialize();
	eManager->SetComponent(entity, transform);

	model_ = Model::Create();
	ModelComp model;
	model.Init("player", model_);
	eManager->SetComponent(entity, model);
	Model* a =
	    ModelManager::GetInstance()->GetModel(eManager->GetComponent<ModelComp>(entity).model_);

	TextureComp texture;
	texture.texture_ = textureHandle_;
	eManager->SetComponent(entity, texture);
}

void GameScene::Update() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0.5f, 2.f);

	/*static uint32_t time = 0;
	if (time++ < 600) {

	}*/

	world->ForEach<ModelComp, VelocityComp>([](ModelComp& model, VelocityComp& velo) {

	});
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
	world->ForEach<ModelComp, TextureComp, TransformComp>(
	    [*this](ModelComp& model, TextureComp& texture, TransformComp& transform) {
		    ModelManager::GetInstance()
		        ->GetModel(model.model_)
		        ->Draw(transform.wTransform_, viewProjection_, texture.texture_);
	    });

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
