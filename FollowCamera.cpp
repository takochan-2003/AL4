#include"FollowCamera.h"

void FollowCamera::Initialize() {
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void FollowCamera::Update() {
	XINPUT_STATE joyState;
	//追従対象がいれば
	if (target_) {
	//追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -10.0f};

		//カメラの角度から回転行列を計算する
	    Matrix4x4 rotateXMatrix = MakeRotateXmatrix(viewProjection_.rotation_.x);
		Matrix4x4 rotateYMatrix = MakeRotateYmatrix(viewProjection_.rotation_.y);
     	Matrix4x4 rotateZMatrix = MakeRotateZmatrix(viewProjection_.rotation_.z);
		Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

		//オフセットをカメラの回転に合わせて回転させる
		offset = TransformNormal(offset, rotateXYZMatrix);

		//座標をコピーしてオフセット分、ずらす
		viewProjection_.translation_ = Add(target_->translation_,offset);
	}
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//回転速度
		const float kCameraSpeed = 0.05f;
		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kCameraSpeed;
	}
	//ビュー行列の更新
	viewProjection_.UpdateMatrix();
}