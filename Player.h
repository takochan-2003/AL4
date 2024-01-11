#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "compute.h"
#include "BaseCharacter.h"

class Player : public BaseCharacter {

	public:

	void Initialize(const std::vector<Model*>&models)override;

	void Update() override;

	void Draw(const ViewProjection& viewProjection) override;

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	void ImGui();

	//浮遊ギミック初期化
	void InitializeFloattingGimmick();

	//浮遊ギミック更新
	void UpdateFlotingGimmick();

	//
	void SetParent(const WorldTransform* parent);

	//移動処理
	void KeyMove();
	void JoyMove();

	//ワールドトランスフォーム取得
	const WorldTransform& GetWorldTransform();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	void ResetPosition();

	private:
	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	//ワールド変換データ
	WorldTransform worldTransform_;

	//自機のワールドデータ
	WorldTransform worldTransformArm_L_;
	WorldTransform worldTransformArm_R_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;

	//1-4の各パーツ毎の3Dモデル
	Model* modelFighterBody_ = nullptr;
	Model* modelFighterHead_ = nullptr;
	Model* modelFighterL_arm_ = nullptr;
	Model* modelFighterR_arm_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//キーボード入力
	Input* input_ = nullptr;

	private:
	//浮遊ギミックの媒介変数
	   float floatingParameter_ = 0.0f;

	   float movePower = 0.0f;

};