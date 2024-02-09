#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include <memory>

#include "Player.h"
#include "Skydome.h"
#include"Ground.h"
#include "Scene.h"

class TitleScene {

	public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	    TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	    ~TitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void sceneReset();

	bool isSceneEnd = false;

	bool IsSceneEnd() { return isSceneEnd; }
	SceneType NextScene() { return SceneType::kGamePlay; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// スプライト
	Sprite* sprite_ = nullptr;
	// 3Dモデル
	std::unique_ptr<Model> model_;
	// スカイドーム
	std::unique_ptr<Skydome> skydome_;
	// スカイドームの3Dモデル
	std::unique_ptr<Model> modelskydome_;
	// 地面
	std::unique_ptr<Ground> ground_;
	// 3Dモデル
	std::unique_ptr<Model> modelGround_;

	// ビュープロジェクション
	ViewProjection viewProjection_;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
};