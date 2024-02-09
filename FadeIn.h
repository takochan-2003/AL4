#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Sprite.h"
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
