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

private:
	std::unique_ptr<Model> model_[4];
	WorldTransform worldTransform_[4];
	Vector4 modelMaterial_;
};

