#include "Skydome.h"
#include <cassert>


void Skydome::Initialize(Model* model, uint32_t textureHndle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHndle;

	input_ = Input::GetInstance();

	worldTransform_.Initialize();

	worldTransform_.scale_ = {60.0f, 60.0f, 60.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
}

void Skydome::Update() { worldTransform_.UpdateMatrix(); }

void Skydome::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}