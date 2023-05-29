#include "WorldTransform.h"

void WorldTransform::UpdateMatrix() {
	matWorld_ = Matrix4x4::Affine(scale_, rotation_, translation_); // 行列を計算する
	if (parent_)
		matWorld_ *= parent_->matWorld_;
	TransferMatrix(); // 定数バッファに転送する
}