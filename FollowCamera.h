#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "compute.h"
#include "Input.h"

class FollowCamera {

	public:

		void Initialize();

		void Update();

		void SetTarget(const WorldTransform* target) { target_ = target; }

		const ViewProjection& GetViewProjection() { return viewProjection_; }

	private:
		//ビュープロジェクションの初期化
		ViewProjection viewProjection_;

		//追従対象
	    const WorldTransform* target_ = nullptr;


};