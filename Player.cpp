﻿#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	//引数からデータを受け取る
	model_ = model;
	textureHandle_ = textureHandle;
	
	//ワールド変換の初期化
	worldTransform_.Initialize();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

}
void Player::Update() {
	//行列を更新
	worldTransform_.UpdateMatrix();

	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	//キャラクターの移動速度
	const float kCharacterSpeed = 0.2f;
	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}

	//座標移動(ベクトルの加算)
	worldTransform_.translation_=Add(worldTransform_.translation_, move);

	//ワールド行列の更新
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();

}
void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
