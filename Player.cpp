#include "Player.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include<math.h>
#include<ImGuimanager.h>

float EaseInBack(float flame){
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return c3 * flame * flame * flame - c1 * flame * flame;
}

void Player::Initialize(const std::vector<Model*>& models) {
	//// NULLポインタチェック
	//assert(modelBody);
	//assert(modelHead);
	//assert(modelL_arm);
	//assert(modelR_arm);
	//// 引数からデータを受け取る
	//modelFighterBody_ = modelBody;
	//modelFighterHead_ = modelHead;
	//modelFighterL_arm_ = modelL_arm;
	//modelFighterR_arm_ = modelR_arm;

	BaseCharacter::Initialize(models);


	//ビュープロジェクションの初期化

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 2.0f, 0.0f};

	worldTransformArm_R_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformArm_R_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformArm_R_.translation_ = {0.6f, 1.0f, 0.0f};

	worldTransformArm_L_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformArm_L_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformArm_L_.translation_ = {-0.6f, 1.0f, 0.0f};

	worldTransformHead_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformHead_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformHead_.translation_ = {0.0f, 1.4f, 0.0f};

	worldTransformHammer_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformHammer_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformHammer_.translation_ = {0.0f, 1.4f, 0.0f};
	
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransformArm_R_.Initialize();
	worldTransformArm_L_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformHammer_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	InitializeFloattingGimmick();
}
void Player::Update() {
	switch (changeState) {

	case Behavior::kRoot:
		BehaviorRootUpdate();
		break;

	case Behavior::kAttack:
		BehaviorAttackUpdate();
	break;
	}

}
void Player::Draw(const ViewProjection& viewProjection) {

	//modelFighterBody_->Draw(worldTransformBody_, viewProjection);
	//modelFighterHead_->Draw(worldTransformHead_, viewProjection);
	//modelFighterL_arm_->Draw(worldTransformArm_L_, viewProjection);
	//modelFighterR_arm_->Draw(worldTransformArm_R_, viewProjection);

	models_[0]->Draw(worldTransformBody_, viewProjection);
	models_[1]->Draw(worldTransformHead_, viewProjection);
	models_[2]->Draw(worldTransformArm_L_, viewProjection);
	models_[3]->Draw(worldTransformArm_R_, viewProjection);
	if (changeState == Behavior::kAttack) {
	models_[4]->Draw(worldTransformHammer_, viewProjection);
	}

}


void Player::ImGui() {
	float head[3] = {
	    worldTransformHead_.translation_.x,
		worldTransformHead_.translation_.y,
	    worldTransformHead_.translation_.z
	};
	float L_arm[3] = {
	    worldTransformHead_.translation_.x,
		worldTransformHead_.translation_.y,
	    worldTransformHead_.translation_.z
	};
	float R_arm[3] = {
	    worldTransformHead_.translation_.x,
		worldTransformHead_.translation_.y,
	    worldTransformHead_.translation_.z
	};

	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head Translation", head, 100.0f, -100.0f);
	ImGui::SliderFloat3("Larm Translation", L_arm, 100.0f, -100.0f);
	ImGui::SliderFloat3("Rarm Translation", R_arm, 100.0f, -100.0f);

	ImGui::End();

	worldTransformHead_.translation_.x = head[0];
	worldTransformHead_.translation_.y = head[1];
	worldTransformHead_.translation_.z = head[2];

	worldTransformArm_L_.translation_.x = L_arm[0];
	worldTransformArm_L_.translation_.y = L_arm[1];
	worldTransformArm_L_.translation_.z = L_arm[2];

	worldTransformArm_R_.translation_.x = R_arm[0];
	worldTransformArm_R_.translation_.y = R_arm[1];
	worldTransformArm_R_.translation_.z = R_arm[2];

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
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floatingPower;
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
	
	    if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			changeState = Behavior::kAttack;
		}
	}
	

}


const WorldTransform& Player::GetWorldTransform() { return worldTransform_; }

void Player::SetParent(const WorldTransform* parent) {
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos = {};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::BehaviorRootUpdate() {

// 移動処理
	// KeyMove();
	JoyMove();
	UpdateFlotingGimmick();

	

	// 親と子の座標を合わせる
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformArm_L_.parent_ = &worldTransformBody_;
	worldTransformArm_R_.parent_ = &worldTransformBody_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformHammer_.parent_ = &worldTransformBody_;

	// ImGui();

	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformArm_R_.UpdateMatrix();
	worldTransformArm_L_.UpdateMatrix();
	worldTransformHammer_.UpdateMatrix();
}

void Player::BehaviorAttackUpdate() {
	attackTime++;
	float kDegreeToRadian = (float)M_PI / 180.0f;
	if (attackTime <= kAttackFinishTime) {
	
		//イージングの変数
	    float flame = (float)(attackTime / kAttackFinishTime);
		float easeInBack = EaseInBack(flame * flame);
		float weaponAngle = (float)((90 * kDegreeToRadian)) * easeInBack;
		float armAngle = (float)((120 * kDegreeToRadian)) * easeInBack;

		//イージングで動かす
		worldTransformHammer_.rotation_.x = weaponAngle;
		worldTransformArm_L_.rotation_.x = armAngle + (float)M_PI;
		worldTransformArm_R_.rotation_.x = armAngle + (float)M_PI;
	
	} else if (attackTime >= kAttackFinishTime) {
		panishTime++;
		if (panishTime >= kPanishTime) {
		attackTime = 0;
		panishTime = 0;

		worldTransformHammer_.rotation_.x = beforeHammerRotation;
		worldTransformArm_L_.rotation_.x = beforeLarmRotation;
		worldTransformArm_R_.rotation_.x = beforeRarmRotation;

		changeState = Behavior::kRoot;
		}
	}

	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformArm_R_.UpdateMatrix();
	worldTransformArm_L_.UpdateMatrix();
	worldTransformHammer_.UpdateMatrix();

}

