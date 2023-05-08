#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <imgui.h>

#include <cassert>
#include <random>

#include "Math.hpp"
#include "AxisIndicator.h"

#include "Header/Component/Component.hpp"
#include "Header/Entity/EntityManager.hpp"
#include "Header/World/World.hpp"

#include "Header/ModelManager/ModelManager.hpp"

GameScene::GameScene() {}

GameScene::~GameScene() {
	if (debugCamera_) {
		delete debugCamera_;
	}
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("uvChecker.png");

	viewProjection_.Initialize();
	debugCamera_ = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);


	world_ = World::GetInstance();

	EntityManager* eManager = world_->GetEntityManager();

#pragma region Entity

	Entity entity =
	    eManager->CreateEntity<Identifier, TransformComp, ModelComp, TextureComp, VelocityComp>();
	Identifier identifier;
	identifier.Init("player");
	eManager->SetComponent(entity, identifier);

	TransformComp transform;
	transform.wTransform_.Initialize();
	eManager->SetComponent(entity, transform);

	ModelComp model;
	model.Init("player", Model::Create());
	eManager->SetComponent(entity, model);
	Model* a =
	    ModelManager::GetInstance()->GetModel(eManager->GetComponent<ModelComp>(entity).model_);

	TextureComp texture;
	texture.texture_ = textureHandle_;
	eManager->SetComponent(entity, texture);

	VelocityComp velocity;
	velocity.velocity_ = Vector3::zero();
	eManager->SetComponent(entity, velocity);

#pragma endregion

}

void GameScene::Update() {

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

#endif // _DEBUG
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		auto& vp = debugCamera_->GetViewProjection();
		viewProjection_.matView = vp.matView;
		viewProjection_.matProjection = vp.matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}

	world_->ForEach<VelocityComp>(
	    [](VelocityComp& velocity) { velocity.velocity_ = Vector3::zero(); });

	world_->ForEach<Identifier, VelocityComp>(
	    [this](Identifier& identifier, VelocityComp& velocity) {
		    const float moveSpeed = 0.4f;
		    if (input_->PushKey(DIK_A)) {
			    velocity.velocity_.x -= 1;
		    } else if (input_->PushKey(DIK_D)) {
			    velocity.velocity_.x += 1;
		    }

		    if (input_->PushKey(DIK_W)) {
			    velocity.velocity_.y += 1;
		    } else if (input_->PushKey(DIK_S)) {
			    velocity.velocity_.y -= 1;
		    }

		    velocity.velocity_ = velocity.velocity_.Nomalize() * moveSpeed;
	    });

	world_->ForEach<TransformComp, VelocityComp>(
	    [](TransformComp& transform, VelocityComp& velocity) {
		    Vector3& scale = transform.wTransform_.scale_;
		    Vector3& rotation = transform.wTransform_.rotation_;
		    Vector3& translation = transform.wTransform_.translation_;
		    translation += velocity.velocity_;

		    const Vector3 Stopper{34, 18, 0};

		    translation.x = std::clamp(translation.x, -Stopper.x, Stopper.x);
		    translation.y = std::clamp(translation.y, -Stopper.y, Stopper.y);

		    transform.wTransform_.matWorld_ = Matrix4x4::Affine(scale, rotation, translation);

		    transform.wTransform_.TransferMatrix();
	    });

	world_->ForEach<TransformComp, Identifier>(
	    [](TransformComp& transform, Identifier& identifier) {
		    Vector3& position = transform.wTransform_.translation_;
		    // float posBuff[3] =
		    ImGui::Begin("player");
		    ImGui::InputFloat3("position", &position.x);
		    ImGui::End();
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
	world_->ForEach<ModelComp, TextureComp, TransformComp>(
	    [this](ModelComp& model, TextureComp& texture, TransformComp& transform) {
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
