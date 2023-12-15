#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "compute.h"
class BaseCharacter {

	protected:
		//モデルデータ配列
	    std::vector<Model*> models_;
		//ワールド変換データ
	    WorldTransform worldTransform_;

	public:
	    virtual void Initialize(const std::vector<Model*>& models);

		virtual void Update();

		virtual void Draw(const ViewProjection& viewProjection);

		const WorldTransform& GetWorldTransform() { return worldTransform_; }

		private:


};