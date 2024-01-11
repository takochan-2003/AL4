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
#include"Enemy.h"
#include"BaseCharacter.h"
#include "Skydome.h"
#include"Ground.h"
#include"FollowCamera.h"
#include "Scene.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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

	void Timer();

		// 衝突判定
	void CheakAllCollisions();

	// シーンのリセット
	void sceneReset();

	//void EnemyMove();

	bool isSceneEnd = false;

	bool IsSceneEnd() { return isSceneEnd; }
	SceneType NextScene() {return SceneType::kTitle;	}

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//ゲームオーバーのフラグ
	bool isGameOver_ = false;
	bool isGameClear_ = false;

	//タイマー
	int time = 0;
	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_; 

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//スプライト
	Sprite* sprite_ = nullptr;

	Sprite* gameoverSprite_ = nullptr;
	uint32_t gameover_ = 0;

	Sprite* clearSprite_ = nullptr;
	uint32_t gameclear_ = 0;
	//3Dモデル
	std::unique_ptr<Model> model_;

	//1-4各パーツごとの3Dモデル
	std::unique_ptr<Model> modelFighterBody_;
	std::unique_ptr<Model> modelFighterHead_;
	std::unique_ptr<Model> modelFighterL_arm_;
	std::unique_ptr<Model> modelFighterR_arm_;

	//敵の3Dモデル
	std::unique_ptr<Model> modelNeedleBody_;
	std::unique_ptr<Model> modelNeedleL_arm_;
	std::unique_ptr<Model> modelNeedleR_arm_;


	//スカイドーム
	std::unique_ptr<Skydome> skydome_;
	//スカイドームの3Dモデル
	std::unique_ptr<Model> modelskydome_;

	//ビュープロジェクション
	ViewProjection viewProjection_;
	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//自キャラ
	std::unique_ptr<Player> player_;

	//敵キャラ
	std::unique_ptr<Enemy> enemy_;

	// 地面
	std::unique_ptr<Ground> ground_;
	// 3Dモデル
	std::unique_ptr<Model> modelGround_;

	//カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
