#include "Enemy.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include<math.h>
#include<ImGuimanager.h>

void Enemy::Initialize(const std::vector<Model*>& models) {
	BaseCharacter::Initialize(models);

	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, -3.0f, 50.0f};

	worldTransformArm_R_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformArm_R_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformArm_R_.translation_ = {0.6f, 1.0f, 0.0f};

	worldTransformArm_L_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformArm_L_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformArm_L_.translation_ = {-0.6f, 1.0f, 0.0f};

		// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransformArm_R_.Initialize();
	worldTransformArm_L_.Initialize();
	worldTransformBody_.Initialize();

}

void Enemy::Update() {

		// 親と子の座標を合わせる
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformArm_L_.parent_ = &worldTransformBody_;
	worldTransformArm_R_.parent_ = &worldTransformBody_;

	

	//移動速度
	//const float speed = 0.1f;

	//worldTransform_.rotation_.y += 0.01f;
	//
	////移動量
	//Vector3 move = {0.0f, 0.0f, speed};
	//
	////回転行列
	//Matrix4x4 matRotY = MakeRotateYmatrix(worldTransform_.rotation_.y);
	//
	////移動量を回転に合わせて回転させる
	//move = TransformNormal(move, matRotY);
	////異ふぉう
	//worldTransform_.translation_ = Add(move, worldTransform_.translation_);

	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformArm_R_.UpdateMatrix();
	worldTransformArm_L_.UpdateMatrix();

}

void Enemy::Draw(ViewProjection& viewProjection) {

	//modelFighterBody_->Draw(worldTransformBody_, viewProjection);
	//modelFighterL_arm_->Draw(worldTransformArm_L_, viewProjection);
	//modelFighterR_arm_->Draw(worldTransformArm_R_, viewProjection);

	models_[0]->Draw(worldTransformBody_, viewProjection);
	models_[1]->Draw(worldTransformArm_L_, viewProjection);
	models_[2]->Draw(worldTransformArm_R_, viewProjection);

}

void Enemy::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

void Enemy::EnemyMove(Vector3 position) {
	Vector3 speed = {0.7f, 0.7f, 0.7f};
	Vector3 subVector = Subtract(position, worldTransform_.translation_);
	subVector = Normalize2(subVector);
	subVector = Multiply(subVector, speed);

	//Add(worldTransform_.translation_, subVector);
	worldTransform_.translation_.x += subVector.x;
	worldTransform_.translation_.z += subVector.z;
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::ResetPosition() {
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, -3.0f, 50.0f};
}
