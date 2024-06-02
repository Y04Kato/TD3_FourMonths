#pragma once
#include "components/3d/Model.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"

class Mountain
{
public:
	void Initialize();

	void Update();

	void Draw(const ViewProjection viewProjection);

	void SetPlayerPos(Vector3 pos) { playerPos_ = pos; }

private:
	std::unique_ptr<Model> mountainModel_[15];
	WorldTransform worldTransform_[15];
	Vector4 modelMaterial_;

	Vector3 playerPos_;
};

