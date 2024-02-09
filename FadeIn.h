#pragma once
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "WorldTransform.h"
class FadeIn {

public:
	void Initialize();

	void Update();

	void Draw();

	void ResetFadeIn();

private:
	Sprite* fadeSprite_ = nullptr;
	Vector4 fadeColor_;
};