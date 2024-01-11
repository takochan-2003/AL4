#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include"ImGuiManager.h"

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

	//ゲームオーバー画面
	gameover_ = TextureManager::Load("gameover.png");
	// スプライト生成
	gameoverSprite_ =
	    Sprite::Create(gameover_, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});

	// クリア画面
	gameclear_ = TextureManager::Load("clear.png");
	// スプライト生成
	clearSprite_ = Sprite::Create(gameclear_, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
	

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//プレイヤーの3Dモデル読み込み
	modelFighterBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));

		// 自キャラモデル
	std::vector<Model*> playerModels = {
	    modelFighterBody_.get(),
		modelFighterHead_.get(),
		modelFighterL_arm_.get(),
	    modelFighterR_arm_.get()};
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

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (isGameOver_ == false) {
			CheakAllCollisions();

			enemy_->EnemyMove(player_->GetWorldPosition());

			skydome_->Update();

			player_->Update();

			enemy_->Update();

			ground_->Update();

			Timer();

			// デバッグカメラの更新
			debugCamera_->Update();
		} else if(isGameOver_==true||isGameClear_==true){
			if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_A&&isGameOver_==true) {

				isSceneEnd = true;
			}
		}
	}



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
	if (isGameOver_ == true) {
		gameoverSprite_->Draw();
	}
	if (isGameClear_ == true) {
		clearSprite_->Draw();
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::Timer() { time++;
	if (time >= 60*20) {
		isGameClear_ = true;
	}
}

void GameScene::CheakAllCollisions() {
	// 対象物A(自機)とB(敵)
	Vector3 posA = {0};
	Vector3 posB = {0};

	// 2間点の距離
	float posAB;

	// 自キャラの半径
	float playerRadius = 1.0f;

	// 敵弾の半径
	float enemyBulletRadius = 5.0f;

#pragma region 自キャラと敵の当たり判定
	// 自キャラのワールド座標を取得
	    posA = player_->GetWorldPosition();

		// 敵のワールド座標を取得
		posB = enemy_->GetWorldPosition();

		// AとBの距離を求める
		posAB = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z);

		float R = (playerRadius + enemyBulletRadius) * (playerRadius + enemyBulletRadius);

		// 球と球との当たり判定
	    if (posAB != 0.0f) {

		if (posAB <= R) {
			isGameOver_ = true;
			player_->ResetPosition();
			enemy_->ResetPosition();
		}
	    }

}
void GameScene::sceneReset() {
	   
		isGameOver_ = false;
	    isGameClear_ = false;
	    time = 0;

	    // シーンの切り替えフラグ
	    isSceneEnd = false;
	   
}
