#include "Player.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include<math.h>

void Player::Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm) {
	// NULLポインタチェック
	assert(modelBody);
	assert(modelHead);
	assert(modelL_arm);
	assert(modelR_arm);
	// 引数からデータを受け取る
	modelFighterBody_ = modelBody;
	modelFighterHead_ = modelHead;
	modelFighterL_arm_ = modelL_arm;
	modelFighterR_arm_ = modelR_arm;


	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransformArm_R_.Initialize();
	worldTransformArm_L_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	//ビュープロジェクションの初期化

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};

	//親子関係を結ぶ
	//worldTransformP_=SetParent(&GetWorldTransform());

	//ズラす量
	Vector3 BodyPosition(0.0f, 3.0f, 0.0f);
	Vector3 HeadPosition(0.0f, 5.0f, 0.0f);
	Vector3 L_armPosition(0.0f, 3.0f, 2.0f);
	Vector3 R_armPosition(0.0f, 3.0f, -2.0f);

	

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	InitializeFloattingGimmick();
}
void Player::Update() {

	// 移動処理
	// KeyMove();
	JoyMove();
	UpdateFlotingGimmick();

	//親と子の座標を合わせる
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformArm_L_.parent_ = &worldTransformBody_;
	worldTransformArm_R_.parent_ = &worldTransformBody_;
	worldTransformHead_.parent_ = &worldTransformBody_;

	worldTransform_.UpdateMatrix();
	worldTransformArm_R_.UpdateMatrix();
	worldTransformArm_L_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();

}
void Player::Draw(ViewProjection& viewProjection) {
	modelFighterBody_->Draw(worldTransformBody_, viewProjection);
	modelFighterHead_->Draw(worldTransformHead_, viewProjection);
	modelFighterL_arm_->Draw(worldTransformArm_L_, viewProjection);
	modelFighterR_arm_->Draw(worldTransformArm_R_, viewProjection);
}


void Player::InitializeFloattingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFlotingGimmick() {
	//フレーム
	const int frame = 120;
	//浮遊移動のサクル<frame>
	const uint16_t period = frame;
	//1フレームでのパラメーター加算値
	const float step = 2.0f * 3.14f / period;

	//パラメーターを1ステップ分加算
	floatingParameter_ += step;
	//2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * 3.14f);

	//浮遊の振幅
	const float floatingPower = 0.2f;
	//浮遊を座標に反映
	worldTransform_.translation_.y = std::sin(floatingParameter_) * floatingPower;
}

void Player::KeyMove() {
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラクターの移動速度
	const float kCharacterSpeed = 0.2f;
	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}
	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
}

void Player::JoyMove() {
	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
	//Vector3 move = {0, 0, 0};
	// ゲームパッド状態取得変数
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 移動速度
		const float kCharacterSpeed = 0.5f;
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed,
		    0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed,
		};

		// カメラの角度から回転行列を計算する
		Matrix4x4 rotateXMatrix = MakeRotateXmatrix(viewProjection_->rotation_.x);
		Matrix4x4 rotateYMatrix = MakeRotateYmatrix(viewProjection_->rotation_.y);
		Matrix4x4 rotateZMatrix = MakeRotateZmatrix(viewProjection_->rotation_.z);
		Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
		
		// 移動量に速さを反映
		move = Multiply(Normalize2(move), kCharacterSpeed);

		move = TransformNormal(move, rotateXYZMatrix);

		
		if (move.z != 0 || move.y != 0) {
			worldTransform_.rotation_.y = std::atan2(move.x, move.z);
		}

		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	}

	// move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
	// move.z += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	//// 座標移動(ベクトルの加算)
	// worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	// 行列を更新
	worldTransform_.UpdateMatrix();
}


const WorldTransform& Player::GetWorldTransform() { return worldTransform_; }

void Player::SetParent(const WorldTransform* parent) {
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

