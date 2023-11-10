#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "compute.h"

class Player {

	public:

	void Initialize(Model* modelBody,Model* modelHead,Model* modelL_arm,Model* modelR_arm);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	//移動処理
	void KeyMove();
	void JoyMove();

	//ワールドトランスフォーム取得
	const WorldTransform& GetWorldTransform();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	private:
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//1-4の各パーツ毎の3Dモデル
	Model* modelFighterBody_ = nullptr;
	Model* modelFighterHead_ = nullptr;
	Model* modelFighterL_arm_ = nullptr;
	Model* modelFighterR_arm_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//キーボード入力
	Input* input_ = nullptr;
};