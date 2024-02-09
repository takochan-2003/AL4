#include "FadeIn.h"
#include"TextureManager.h"

void FadeIn::Initialize() {
//初期化処理
	uint32_t fadeTexHandle = TextureManager::Load("fade.png");
    fadeSprite_ = Sprite::Create(fadeTexHandle, {0, 0});
	fadeColor_ = {1.0f, 1.0f, 1.0f, 1.0f};
}

void FadeIn::Update() {
	fadeColor_.w -= 0.003f;
	fadeSprite_->SetColor(fadeColor_);
}

void FadeIn::Draw() { fadeSprite_->Draw(); }

void FadeIn::ResetFadeIn() {
	if (fadeColor_.w <= 0.0f) {
		fadeColor_.w = 1.0f;
	}
}
