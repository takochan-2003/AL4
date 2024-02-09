#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Scene.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Skydome.h"
#include"Ground.h"

class TitleScene {

public: 

	TitleScene();

	~TitleScene();

	void Initialize();

	void Update();

	void Draw();

	void sceneReset();

	bool isSceneEnd = false;

	bool IsSceneEnd() { return isSceneEnd; }
	SceneType NextScene() { return SceneType::kGamePlay; }

	private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

		// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// 3Dモデル
	std::unique_ptr<Model> model_;
	// スプライト
	Sprite* sprite_ = nullptr;
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

		// スプライト
	Sprite* spriteTitle_ = nullptr;
};
