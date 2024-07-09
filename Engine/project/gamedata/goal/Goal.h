#pragma once
#include "components/3d/Model.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"

#include "datas/datas.h"

class Goal
{
public:
	void Initialize();

	void Update();

	void Draw(const ViewProjection viewProjection);

private:
	std::unique_ptr<Model> model_[2];
	WorldTransform worldTransform_[2];
	Vector4 modelMaterial_;


	//Datas
	Datas* datas_;
};

