#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();






	
	// スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, {50, 50});
	// 3Dモデルの生成
	model_.reset(Model::Create());

	

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//プレイヤーの3Dモデル読み込み
	modelFighterBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));
	modelHammer_.reset(Model::CreateFromOBJ("hammer", true));

		// 自キャラモデル
	std::vector<Model*> playerModels = {
	    modelFighterBody_.get(),
		modelFighterHead_.get(),
		modelFighterL_arm_.get(),
	    modelFighterR_arm_.get(),
		modelHammer_.get()
	};
	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	player_->Initialize(playerModels);

	//敵キャラの3Dモデル読み込み
	modelNeedleBody_.reset(Model::CreateFromOBJ("needle_Body", true));
	modelNeedleL_arm_.reset(Model::CreateFromOBJ("needle_L_arm", true));
	modelNeedleR_arm_.reset(Model::CreateFromOBJ("needle_R_arm", true));

		// 敵キャラモデル
	std::vector<Model*> enemyModels = {
	    modelNeedleBody_.get(),
		modelNeedleL_arm_.get(),
		modelNeedleR_arm_.get()};

	//敵キャラの生成
	enemy_ = std::make_unique<Enemy>();
	//敵キャラの初期化
	enemy_->Initialize(enemyModels);

	// スカイドームの読み込み
	modelskydome_.reset(Model::CreateFromOBJ("skydome", true));
	// スカイドームの生成
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelskydome_.get(), textureHandle_);

	// 地面
	//   3Dモデルの生成
	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	// 地面の生成
	ground_ = std::make_unique<Ground>();
	// 地面の初期化
	ground_->Initialize(modelGround_.get());

	// カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();
	// カメラの初期化
	followCamera_->Initialize();
	// 自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

	//自キャラに追従カメラをアドレス渡し
	player_->SetViewProjection(&followCamera_->GetViewProjection());

#ifdef _DEBUG

	// デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
#endif // _DEBUG
}

void GameScene::Update() {

	// デバッグカメラの更新
	debugCamera_->Update();

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_C) && isDebugCameraActive_ == false) {
		isDebugCameraActive_ = true;
	}

	if (input_->TriggerKey(DIK_V) && isDebugCameraActive_ == true) {
		isDebugCameraActive_ = false;
	}

	// カメラの処理
	if (isDebugCameraActive_ == true) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {

		followCamera_->Update();
		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
		// ビュープロジェクション行列の更新と転送
		// viewProjection_.UpdateMatrix();
	}
#endif // _DEBUG

	skydome_->Update();

	player_->Update();

	enemy_->Update();

	ground_->Update();
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
	// プレイヤーの描画
	player_->Draw(viewProjection_);

	enemy_->Draw(viewProjection_);

	// 天球の描画
	skydome_->Draw(viewProjection_);

	// 床の描画
	ground_->Draw(viewProjection_);

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
