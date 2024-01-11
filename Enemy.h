#pragma once

#include "BaseCharacter.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "compute.h"
#include "Player.h"

class Enemy : public BaseCharacter {

public:
	void Initialize(const std::vector<Model*>& models) override;

	void Update();

	void Draw(ViewProjection& viewProjection);

	void SetParent(const WorldTransform* parent);

	void EnemyMove(Vector3 position);

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	void ResetPosition();

	// ワールドトランスフォーム取得
	//const WorldTransform& GetWorldTransform();

	// ワールド座標を取得
	//Vector3 GetWorldPosition();

private:
	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// 敵のワールドデータ
	WorldTransform worldTransformArm_L_;
	WorldTransform worldTransformArm_R_;
	WorldTransform worldTransformBody_;

	// 1-4の各パーツ毎の3Dモデル
	Model* modelFighterBody_ = nullptr;
	Model* modelFighterL_arm_ = nullptr;
	Model* modelFighterR_arm_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

};