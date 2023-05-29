#pragma once
#include "Header/ModelManager/ModelManager.h"
#include <WorldTransform.h>

enum class CollisionFilter {
	Player = 0b00001,
	Enemy = 0b00010
};

class Collider {
	// 衝突属性(自分)
	uint32_t collisionAttribute_ = 0xFFFFFFFF;
	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xFFFFFFFF;

protected:
	WorldTransform worldTransform_;
	float radius = 0.5f;

public:
	/// @brief 衝突属性を取得
	/// @return 衝突属性
	const uint32_t& GetCollisionAttribute() const { return collisionAttribute_; }
	/// @brief 衝突属性を設定
	/// @param newAttribute 衝突属性
	void SetCollisionAttribute(const uint32_t& newAttribute) { collisionAttribute_ = newAttribute; }
	/// @brief 衝突マスクを取得
	/// @return 衝突マスク
	const uint32_t& GetCollisionMask() const { return collisionMask_; }
	/// @brief 衝突マスクを設定
	/// @param newMask 衝突マスク
	void SetCollisionMask(const uint32_t& newMask) { collisionMask_ = newMask; }

	const float& GetRadius() const { return radius; }
	void SetRadius(const float& rad) { radius = rad; }

	void AddPosition(const Vector3& vector) {
		worldTransform_.translation_ += vector;
		worldTransform_.UpdateMatrix();
	}
	const Vector3& GetPosition() const { return worldTransform_.translation_; }
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	virtual void OnCollision() = 0;
};

class Entity : public Collider {
protected:
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Vector3 velocity_ = {};

public:
	Model* GetModelPtr() const { return model_; }

	virtual ~Entity() {}

	virtual void Draw(const ViewProjection& Vp) {
		model_->Draw(worldTransform_, Vp, textureHandle_);
	}

	// virtual void Init(const std::string& modelKey, const uint32_t& textureHandle);
	virtual void Init(Model* model, const uint32_t& textureHandle);

	virtual void Update() {}

	void Move() {
		worldTransform_.translation_ += velocity_;
		worldTransform_.UpdateMatrix();
	}
};